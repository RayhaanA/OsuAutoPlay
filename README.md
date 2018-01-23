osu! auto play mode implementation. Not intended for use during online play or for submitting scores made by the bot. Play osu! at: [osu.ppy.sh](https://osu.ppy.sh)

Current status: tapping is generally 100% percent accurate, aiming... not so much (~50%). Needs work on generating the sliders and mapping object coordinates from osu!pixels to Windows coordinates.

Example of the "tapping" functionality in this video! (Sorry about the jitteriness (and the song if it's not your thing! If that's the case hopefully you can get the jist of it without sound)):

[![osu!AutoplayBot](https://img.youtube.com/vi/ELEkiWyPBXE/0.jpg)](https://www.youtube.com/watch?v=ELEkiWyPBXE)

The game in its essence is moving your mouse to the note and tapping/clicking at the right time (denoted by an "approach circle" closing in around the note) to the beat of a song. 

In this video, it's playing with the osu! game mods AutoPilot, Hidden, Hard Rock, Double Time, and Flashlight. AutoPilot so the bot can focus on "tapping". Hidden makes the notes invisible just before you're about to hit them and removes the timing approach circle, Hard Rock reduces circle size and makes the range for a perfectly timed note hit tighter, double time increases the speed by 1.5x (yeah), and flashlight reduces the visibilty radius to a region around the cursor location. 

All of this put together, while making it harder to watch as a viewer, should hopefully show off the capabilities of the bot when it comes to "tapping". For humans, it's (likely) impossible to be able to pull this off. 

For people who don't know much about osu! it might be a lot to take in, and maybe comparing against a more normal game replay would be helpful.
