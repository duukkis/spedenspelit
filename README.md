# Speden Spelit

## Parts
- Arduino Uno
- Relay Shield - relays will use D4, D5, D6 and D7 : https://www.partco.fi/fi/arduino/arduino-lisaekortit/19714-ard-mcs01584r.html
- 4 Big buttons with leds. For example these: https://www.adafruit.com/product/1192
- Power source for leds (12V). 
- 4 pieces 10k ohm resistors for end of buttons
- TM1637Display: https://www.aliexpress.com/item/32224154853.html?storeId=1414081&spm=2114.12010612.8148356.1.3d6c7436j2xltY

## Relay shield
Just put it on, will occupy D4, D5, D6 and D7

## Button leds wiring
From 12V power source the ground is split into 4 and each one goes directly to led *Not the blue but the other*, the current also split to 4 and each goes to NO1- Normally Open port in each relay, from the relay shield the coms go to leds *marked blue*. (The leds have built in resistor, so no resistor there is needed)

## Button button wiring
From the Arduino 5V is split to 4 and each goes to one of the buttons. From the other end of button it is split so that the other one goes to D2, D3, D9 and D10 and the other split goes to 10k ohm resistor and after that to ground

## TM1637 wiring
Power from 3.3V, ground to ground, CLK to D11 and DIO to D10

Compile and upload the software to Arduino and see what happens.
