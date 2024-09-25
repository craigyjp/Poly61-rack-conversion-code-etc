# Poly61-rack-conversion-code-etc

I built the Tauntek replacement CPU board for my Poly61 to add MIDI and extra options, better parameter resolution etc. 

Then I thought I could buy some Poly 61 boards and build another CPU board and try and build another 61 into a rack as a module rather than a keyboard. 
I bought a bunch of boards from Germany, unfortunately the anti board never arrived, but I got a voice board, clock board, rear panel and ARP board. 
Based on these I built a new front panel using the Korg schematics which included my version of the ARP board but with buttons instead of slide switches. 
I had some faults when I first powered it up, swapped power cable, missing power to an opamp etc. 
So after resolving these I had a bad sound from voices 1 & 6, these took a while to resolve as it turned out I had used the wrong resistor on my anti board build. 
It sounds great now, I finished the wiring and a new front panel with a param display etc. 

I created two schematics using the Arduino Nano, the first one emulates the slide switches of the Arp control and replaces them with LED buttons such as PB86-A1 and A2.
The second schematic receives MIDI on channel 1 only currently and generates a CV from the pitchbend to feed into the modulation input of the Poly61 clock board.
And it also controls two channels of a quad VCA to allow the modulation LFO to go to the modulation input of the clock board and the cutoff frequency of the filters.
Thus providing FM and TM on CC numbers 2 and 3, there is already FM on CC 1 as normal modulation but it is not very strong.


