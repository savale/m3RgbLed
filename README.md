m3RgbLed
========

Firmware for an ARM cortex m3 (lpc1343) galago board controlling multiple tlc5940 chips with rgb leds.

Current state of the project:

o) using bitbang method for controlling the tlc5940 chips.
o) simple pattern is shown: 
[![ScreenShot](https://raw.github.com/GabLeRoux/WebMole/master/ressources/WebMole_Youtube_Video.png)](http://www.youtube.com/watch?v=SP51AXibXsw)

Next steps:

1) Implement Interrupt based control for the tlc5940 GSCLK
2) Implement SPI for sending the data
3) refactor the code
