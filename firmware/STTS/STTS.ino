//         ___________________________________________________________________________________________
//        /                  |                         |                           /                  |
//       /                   |                         |                          /                   |
//      /                    |                         |                         /                    |
//     /              _______|_______           _______|_______         ________/              _______|
//    /              |               |         |              |         |      /              |
//    |              |_______        |         |              |         |      |              |_______       
//    |                      |       |         |              |         |      |                      |
//    |________              |       |         |              |         |      |________              |
//             |             |       |         |              |         |               |             |
//    _________|             /       |         |              |         |       ________|             /
//    |                     /        |         |              |         |      |                     /
//    |                    /         |         |              |         |      |                    /
//    |                   /          |         |              |         |      |                   /          STTS pocket synth firmware
//    |__________________/           |_________|              |_________|      |__________________/           v0.1

// ---------- Pins ----------
struct Pad {
  VPORT_t *vp;   
  uint8_t m;     
};


const Pad pads[16] = {
  {&VPORTB, PIN1_bm},  // 0  C
  {&VPORTB, PIN2_bm},  // 1  C#
  {&VPORTB, PIN3_bm},  // 2  D
  {&VPORTB, PIN4_bm},  // 3  D#
  {&VPORTB, PIN5_bm},  // 4  E
  {&VPORTB, PIN6_bm},  // 5  F
  {&VPORTB, PIN7_bm},  // 6  F#
  {&VPORTA, PIN7_bm},  // 7  G
  {&VPORTA, PIN6_bm},  // 8  G#
  {&VPORTA, PIN5_bm},  // 9  A
  {&VPORTA, PIN4_bm},  // 10 A#
  {&VPORTA, PIN3_bm},  // 11 B
  {&VPORTA, PIN2_bm},  // 12 high C
  {&VPORTC, PIN2_bm},  // 13 OCT_DN
  {&VPORTC, PIN3_bm},  // 14 OCT_UP
  {&VPORTA, PIN1_bm},  // 15 MODE
};

#define NUM_PADS   16
#define NUM_KEYS   13
#define PAD_OCT_DN 13
#define PAD_OCT_UP 14
#define PAD_MODE   15

#define SEND_M   PIN1_bm  
#define PIEZO_A  PIN0_bm   
#define PIEZO_B  PIN0_bm 

// ---------- Settings ----------
#define SAMPLES   16         
#define TIMEOUT   1000     
#define SLEEP_MS  30000UL    

// ---------- State ----------
volatile bool soundOn = false;
uint16_t baseline[NUM_PADS];
int8_t octave = 0;          
uint8_t mode = 0;           
uint32_t lastTouch = 0;
bool prevFn[3] = {false, false, false};


const uint16_t freq100[12] = {
  26163, 27718, 29366, 31113, 32963, 34923,
  36999, 39200, 41530, 44000, 46616, 49388
};

// ================= SPEAKER =================


ISR(TCB0_INT_vect) {
  TCB0.INTFLAGS = TCB_CAPT_bm;
  VPORTB.IN = PIEZO_A;  
  VPORTC.IN = PIEZO_B;
}

uint16_t ticksFor(int8_t semis) {      
  int8_t oct = 0;
  while (semis < 0)   { semis += 12; oct--; }
  while (semis >= 12) { semis -= 12; oct++; }
  uint32_t t = (F_CPU / 4) * 100UL / freq100[(uint8_t)semis];
  if (oct > 0) t >>= oct;
  if (oct < 0) t <<= -oct;
  if (t > 65000) t = 65000;
  if (t < 100) t = 100;
  return (uint16_t)t;
}

void soundStart(uint16_t ticks) {
  if (!soundOn) {
    VPORTB.OUT |= PIEZO_A;    
    VPORTC.OUT &= ~PIEZO_B;
    TCB0.CCMP = ticks;
    TCB0.CNT = 0;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm;
    soundOn = true;
  } else {
    cli();
    TCB0.CCMP = ticks;
    if (TCB0.CNT >= ticks) TCB0.CNT = 0;
    sei();
  }
}

void soundStop() {
  TCB0.CTRLA = 0;
  TCB0.INTCTRL = 0;
  soundOn = false;
  VPORTB.OUT &= ~PIEZO_A;
  VPORTC.OUT &= ~PIEZO_B;
}

// ================= TOUCH =================

void waitQuiet() {
  if (!soundOn) return;
  if (TCB0.CCMP < 450) return;   
  while (true) {
    uint16_t c = TCB0.CNT;
    uint16_t top = TCB0.CCMP;
    if (c > 150 && (uint16_t)(top - c) > 250) return;
  }
}

uint16_t sensePad(uint8_t i) {
  VPORT_t *vp = pads[i].vp;
  uint8_t m = pads[i].m;
  uint16_t total = 0;

  for (uint8_t s = 0; s < SAMPLES; s++) {
    waitQuiet();
    cli();
    VPORTC.OUT &= ~SEND_M;    
    vp->OUT &= ~m;                  
    vp->DIR |= m;
    delayMicroseconds(4);
    vp->DIR &= ~m;              

    uint16_t t = 0;
    VPORTC.OUT |= SEND_M;         
    while (!(vp->IN & m) && t < TIMEOUT) t++;
    vp->OUT |= m;                 
    vp->DIR |= m;
    vp->DIR &= ~m;
    vp->OUT &= ~m;
    VPORTC.OUT &= ~SEND_M;          
    while ((vp->IN & m) && t < 2 * TIMEOUT) t++;
    sei();

    total += t;
  }
  return total;
}

uint16_t threshold(uint8_t i) {
  return baseline[i] / 8 + 8;       
}

void calibrate() {
  for (uint8_t i = 0; i < NUM_PADS; i++) baseline[i] = sensePad(i);
  for (uint8_t k = 0; k < 7; k++) {
    for (uint8_t i = 0; i < NUM_PADS; i++) {
      baseline[i] = (uint16_t)(((uint32_t)baseline[i] * 3 + sensePad(i)) / 4);
    }
  }
}

// ================= SLEEP =================

ISR(RTC_PIT_vect) {
  RTC.PITINTFLAGS = RTC_PI_bm;    
}

void goToSleep() {
  soundStop();

  VPORTC.OUT &= ~SEND_M;
  for (uint8_t i = 0; i < NUM_PADS; i++) {
    pads[i].vp->OUT &= ~pads[i].m;
    pads[i].vp->DIR |= pads[i].m;
  }

  // Baseline for MODE with the other pads held low
  pads[PAD_MODE].vp->DIR &= ~pads[PAD_MODE].m;
  uint16_t sleepBase = sensePad(PAD_MODE);
  uint16_t sleepThr = sleepBase / 8 + 8;
  pads[PAD_MODE].vp->DIR |= pads[PAD_MODE].m;

  while (RTC.STATUS > 0) {}
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
  while (RTC.PITSTATUS > 0) {}
  RTC.PITINTCTRL = RTC_PI_bm;
  RTC.PITCTRLA = RTC_PERIOD_CYC8192_gc | RTC_PITEN_bm;

  uint8_t hits = 0;
  while (hits < 2) {              
    SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm;
    __asm__ __volatile__("sleep");
    SLPCTRL.CTRLA = 0;

    pads[PAD_MODE].vp->DIR &= ~pads[PAD_MODE].m;
    uint16_t r = sensePad(PAD_MODE);
    pads[PAD_MODE].vp->DIR |= pads[PAD_MODE].m;

    if (r > sleepBase + sleepThr) hits++;
    else hits = 0;
  }

  // Awake again
  while (RTC.PITSTATUS > 0) {}
  RTC.PITCTRLA = 0;
  for (uint8_t i = 0; i < NUM_PADS; i++) {
    pads[i].vp->DIR &= ~pads[i].m; 
  }
  prevFn[2] = true;               
  lastTouch = millis();

  soundStart(ticksFor(12));          
  delay(60);
  soundStop();
}

// ================= MAIN =================

void setup() {
  VPORTC.DIR |= SEND_M;
  VPORTC.OUT &= ~SEND_M;
  VPORTB.DIR |= PIEZO_A;
  VPORTC.DIR |= PIEZO_B;
  PORTC.PIN4CTRL = PORT_PULLUPEN_bm; 
  PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
  for (uint8_t i = 0; i < NUM_PADS; i++) {
    pads[i].vp->DIR &= ~pads[i].m;
  }

  soundStart(ticksFor(0));  delay(80);
  soundStart(ticksFor(7));  delay(80);
  soundStart(ticksFor(12)); delay(120);
  soundStop();

  calibrate();                    
  lastTouch = millis();
}

void loop() {
  uint16_t raw[NUM_PADS];
  bool touched[NUM_PADS];
  bool any = false;

  for (uint8_t i = 0; i < NUM_PADS; i++) {
    raw[i] = sensePad(i);
    touched[i] = raw[i] > baseline[i] + threshold(i);
    if (touched[i]) {
      any = true;
    } else if (raw[i] < baseline[i] + threshold(i) / 2) {
      // slowly follow temperature and battery drift
      int32_t diff = (int32_t)raw[i] - (int32_t)baseline[i];
      baseline[i] = (uint16_t)((int32_t)baseline[i] + diff / 16);
    }
  }

  bool fn[3] = {touched[PAD_OCT_DN], touched[PAD_OCT_UP], touched[PAD_MODE]};
  if (fn[0] && !prevFn[0] && octave > -2) octave--;
  if (fn[1] && !prevFn[1] && octave < 2)  octave++;
  if (fn[2] && !prevFn[2]) mode = (mode + 1) % 3;
  for (uint8_t k = 0; k < 3; k++) prevFn[k] = fn[k];

  int8_t best = -1;
  uint16_t bestD = 0;
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (touched[i] && raw[i] - baseline[i] > bestD) {
      bestD = raw[i] - baseline[i];
      best = i;
    }
  }

  if (best >= 0) {
    int8_t semis = best + octave * 12;
    if (mode == 1) {                               
      const int8_t arp[4] = {0, 4, 7, 12};
      semis += arp[(millis() / 70) % 4];
    }
    uint16_t t = ticksFor(semis);
    if (mode == 2) {                                  
      int16_t v = (millis() / 8) % 32;
      v = (v < 16) ? v : 31 - v;
      t = (uint16_t)((int32_t)t + (int32_t)t * (v - 8) / 512);
    }
    soundStart(t);
  } else {
    soundStop();
  }

  if (any) lastTouch = millis();
  if (millis() - lastTouch > SLEEP_MS) goToSleep();
}
