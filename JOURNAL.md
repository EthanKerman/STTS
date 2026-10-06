---
STTS super thin tiny synth
EthanKerman
Credit card sized synth for your walled
started 2026-10-01
---

--------------------------

**Oct 1 1 hour 15 min**

--------------------------

Whoop Whoop, off to a strong start

After some parts snooping and cutting, i have decided i will prioritize the overall charm and convenience of it being as thin as reasonably possible like a credit card.

I wont be using conventional buttons and instead little copper touch pads. Should make it much much smaller and easy to slide in and out of a wallet.

I figured out that with my babeh 2016 coin cell battery if i only play 20 mins a day, the battery lasts almost 3 months. And they say size matters...

I decided this time i wouldn't make my schematic an abomination so i used labels. Looks 1000x better than any of my other schematics.

Overall happy so far, i'm really excited to add silkscreen art on the front or back. Maybe both?

Next up is finishing the footprints and starting the layout.

--------------------------

**Oct 2 2 hour 23 mins**

--------------------------

Oh boy, fun stuff.

Was working my way through the JLCPCB website trying to find the C number addresses for each of the parts. Got a good chunk through.

Decided i will no longer be using a tactile button on the front for a wake button. Even if i used a thin one like 2mm, it would still be a substantial bump and i dont want it to take up my whole wallet. So instead i decided to sacrifice some battery life for no wake button and instead will just leave it as open pads in case if i want to solder it on later.

The solution to the button being gone is mostly firmware, but im thinking the ATtiny can scan for any buttons being pressed once every few sec. To wake you just hold a button. Should be interesting to see how this affects the battery life but i can probably optimize the firmware later.

Good stuff so far, cant wait to finish setting it up for PCBA and the footprints and move onto the actual layout part.

Yuh, we finally got done with the schematic and now onto the fun part.

After playing around with easyeda i finally got all the footprints added and am feeling good about it.

I added changed the sizes of some of the pads to make it more pianoie?

I think they will have to either change locations or shape/size eventually because i am having some trouble fitting the through-hole for the little piezo speaker. Thats ok though because i want this little guy to have good volume.

I am really hoping i still have enough room on the front to do some fun silkscreen art. I thought it would be funny to do a silly drawing of my friend peering into the hole of the speaker or sum. If nothing else i will likely have room on the back.

Happy about where this is going so far though.

Next up is the speaker position and next will be routing everything.

Now this is the good part.

Had to hit a quick session to try and finish up the rough placement of all the parts. I kind of just threw the 2-17 resistors next to the ATtiny for now just as a placeholder.

Love routing though, probably the best part next to the art.

Also added the thru hole for the speaker and made parts fit around it while trying to avoid parts overlapping the keys because i think that can make them less sensitive or something?

Yippee though, cant wait to continue.

--------------------------

**Oct 3 30 mins**

--------------------------

Hmmm, im realizing i need to study PCB routing theory.

This reminds me of that game i played on the ipad where you connect the dots and stuff. Bummer there isnt a bridge version for PCBs

Only a short session for now but hoping to get more done.

I think ill have to redo most of this routing after i figure out how to do it properly but i think playing around with it helped me to become more familiar with it. I can at least say i understand it better.

I am a learner by doer so its kind of how things run sometimes.

--------------------------

**Oct 4 **2 hours 50 mins**

--------------------------

hehe 6... oh wait not yet

I really like the routing part because your not just looking up pinouts. This is my first time getting this far so i am learning A TON.

Ex. you want to put the parts close to both of the connections instead of just crowded around the micro controller. I found it much much easier to do everything when the resistors were close to my keys rather all in one spot

I enjoy learning by doing so this is great. I love the trial and error, its kind of like a game.

Overall this part is kind of like a fun grind because im learning alot but as of now will refuse to look up a guide because its kind of fun seeing how the placement of parts and hte routing itself is changing as i make more connections.

Next up is just finishing connecting the keys to the ATtiny, i think i only have 2 or 3 more so pretty close.

Whats that? this is devlog 7? 6... 7?

Yessir, got all the keys routed. Still learning a very large amount. I was kind of bummed, the plane wifi dropped for a bit and didnt realize lapse had an error and lost about 45 mins of work. oh well.

I am now having trouble with the last 4 connections. 2 being the little piezo speakers and 1 being part of the keys and the other is a programing pin. I could technically take out the programing pin and make it substantially easier but i was planning on CAding and printing a nice little jig so actually programing the ATtiny would be 1000x easier.

I going to try to wrestle with it a bit longer then might make that change.

Overall im still liking this stake and am continuing to learn things like utilizing the resistor gap to get wires across. good stuff

Glad i finished the keys but theres a pretty high chance thing get shifted around.

Wanted to get a bit done between studying and so i did a quick session and continued working on routing everything. I have 2 pads off of U1 and their keys but cant seem to match them up. As well as the piezo buzzers and UPDI programing pin are still hanging loose.

I think ill just conform and look up a tutorial on how to route everything properly. Might speed up the process.

Overall still making progress though.

Also didnt realize you cant annotate in Fedora WS image viewer so use your imagination. (its the rats nest from the piezo to the loose route)

--------------------------

**Oct 5 2 hours 49 mins**

--------------------------

Ok. I have folded.

Decided to actually look up the method of how to do routing and as it turns out i was doing it all wrong. Probably could have saved me a few hours to just start here but i learned my lesson. It really makes sense in retrospect how your supposed to treat it like a waterfall and also order the pins of the microcontroller to make routing as easy as possible.

I first went back and changed the pin out of the ATtiny then started moving parts around to make the layout easier and also got rid of the wake button and debugging LED because i could just use the piezo speaker as the alt to the LED and the MODE surface was already going to be the wake, the button was just a back up.

I also went ahead and added some labels on the back after my design rules check came out clean. Then started working on some art. I found a goofy photo of my friend who loves music so i decided he would be a good target to put on here.

Overall happy with how its shaping up and i am continuing to learn alot. Next up is finishing the art and seeing how much it will actually cost for sure.

Double digits baby

Got the art done on the front. Looks just as goofy as i was hoping.

I also double checked everything one last time and exported everything to make sure it all works in JLC and its all good to go.

I asked in Slack whether i have to have my firmware done before i try and get it approved so that will determine whats next.

--------------------------

**Oct 6 3 hours 3 mins**

--------------------------

well that should pretty much wrap it up.

I made v0.1 firmware and am super excited to test it and see how it goes. There is not a ton to write, i had a little bit of help from a friend of mine who works with synths which was nice because the modulation and stuff was all new to me.

Super curious to see how this piezo speaker works out, ive watched a few videos and they seem super sweet. I found a guy who used differential drives for his and got almost 2x the sound so i made sure to add it to the firmware.

I also got tired and wanted a break so i made this nice little header which i think looks cool and makes it look like those other official firmware. :D

Next up if polishing the github repo which i just made and adding the journal and read me. The boring stuff.

Realized i was cutting it close on credits for the project so i decided to whip up a quick case so that the back components are protected.

granted you wouldn't need it if its in a wallet and you probably wouldnt want it either considering it adds a few mm. I think i will use it though.

Thought the Orpheus was a nice little touch.

Next i might make a front cover version too?

Figured only after i ended my lapse that i could make a full cover version too. It just slides on after you've put on the bottom cover.

I actually like this one a little bit better in CAD but it always looks different after printing so ill have to test them out.

I think after i update the repo its pretty much ready for review.


--------------------------|
                           \
================================================

**Grand total hours: ~12 hours 45 min**

================================================
                           /
--------------------------|
