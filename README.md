# HapticCompass
Arduino project for HapticCompass belt

Using a compass to trigger the north-most haptic engine once in X seconds, with proper power-saving sleep in between.
Using a 1000mhA 5V portable charger and 4 button-sized haptic engines on an Arduino Nano V3 I was able to run it for 20 hours.

This code assumes using a HMC5883 compass and accelometer chip. I used a genreic one and it worked well.
For correct North, declinationAngle should be changed to match your position on the globe.
the array actions[] holds the output pins the engines are connected to. you could use any number of outputs. they are ordered from north clockwise.

I'm not electrical engineer, but ALWAYS CONNECT ENGINES WITH A TRANSISTOR!
