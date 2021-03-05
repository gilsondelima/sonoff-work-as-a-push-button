# sonoff-work-as-a-push-button
Sonoff RE5V1C 5V (Based on ESP8285), work as a push button, powered by SinricPro library

# Working
In this project sonoff realy work as a "push button switch" with an "on" and "off" position. 
In the "on" state, the relay is triggered for 0.5 seconds to power on the device.
And relays in the "off" state behave the same as in the "on" state, make a 0.5 secound pulse to turn off a device. 

# Connection
Access the serial interface by soldering wiring to the labeled solder pads (3V3, ERX, ETX, GND) are available on the relay mounted side of the PCB. Note: use the ERX and ETX pins, not the two marked RX/TX. 
In this connection image, I used 5v instead of 3V3 because Sonoff RE5V1C have an inbuild LM1117 voltage regulator. which can deliever up to 800-mA, 3v3.
To Program the ESP8285 i used FTDI FT232RL.

![sonoff connection](https://user-images.githubusercontent.com/77229506/110124905-098bea00-7e06-11eb-8299-4438df0a30cc.jpg)


