README

This project based on PlatformIO SDK (http://platformio.org).

Hardware:
- WeMoS D1 mini (http://www.wemos.cc/)
- COZIR Ambient CO2 Sensor (GC-0012 - http://www.co2meter.com/products/cozir-0-2-co2-sensor)

Installation

Install PlatformIO.
Install CH34x Driver.


PlatformIO Dependencies

platformio lib install 168 - ESP SoftwareSerial
platformio lib install 550 - ThingSpeak

Sensors Firmware Upgrade

platformio run -e d01 -t upload
platformio run -e d02 -t upload
platformio run -e d03 -t upload


Calibration

D0 to 3.3V for 20 seconds or more


Real-time Output

D01 - https://thingspeak.com/channels/147559
D02 - https://thingspeak.com/channels/148621
D03 - https://thingspeak.com/channels/148675

