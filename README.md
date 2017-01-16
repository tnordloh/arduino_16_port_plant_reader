# Plant moisture sensor, with up to 16 ports #

This moisture sensor uses arduino's analog sensor, to detect when a plant needs
to be watered.  Placing two wires into the plant soil, then sending a charge
through the soil will produce a number between 0 and 1023.  With some
experimentation, we picked "550" as dry, and "650" as REALLY dry.  

he Arduino we're using has only 6 analog ports, so we are extending the 
capabilities, by using two 
[SparkFun Analog/Digital MUX Breakout - CD74HC4067](https://www.sparkfun.com/products/9056).

the MUX (aka multiplexer) uses five ports, but can create access to 16 additional
ports.  We use two of these; one to apply power, the other to read how much 
power gets through.  

Additionally, each of the moisture sensors has an LED built into it, so that we
know which of the 16 plants require watering.  To address the 16 lights, we 
are using two
[SN74HC595 8-Bit shift registers](https://www.sparkfun.com/products/13699), 
which consumes three more ports on the Arduino.

Since we had a couple of ports left over, we wired up a small lcd screen, so that
we can visually verify what each plant is actually registering.  This was 
just for fun, but it could be used to set each individual plant to its' own
level of dryness, to make up for differences between each sensor, or differences
in soil, that might make the moisture sensors inaccurate.


