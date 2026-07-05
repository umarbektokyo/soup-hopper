# Soup-Hopper
The world is ending on July 7th... and in the ending of the world, soup turns into a can! Imagine if this soup-hopper could ride around, shooting into people (and otherworldy creatures that are ending the world) with a powerful water gun.
![demo](assets/demo.png)
# BOM
## Soup-Hopper
- Aluminium Can
- LXN 18650 2000mAh - 7.4V 14.8Wh
- ESP32 XIAO C3
- MPU-6050
The main body is made of an aluminium can. On both sides, there is a motor (2207), an MPU-6050, ESP32 XIAO C3. Additionally, it will be able to sprink water, but that is a separate system for later. The goal with the base Soup-Hopper's form is to be able to ride forward/backward while the orientation of the robot stays the same. The controls are: turning right/left (wheels turn opposite dimensions) and also turning up/down (looking up and down whilst in-place idk how if we have only two motors).
Wiring for Soup-Hopper:
- ESP32 to MPU6050:
  GND - GND
  3V3 - VCC
  D4 - SDA
  D5 - SDL
- ESP32 to 3.7V Battery:
  minus - black
  plus - red
- ESP32 to Relay:
  GND - GND
  D2 - Input
  3V3 - VCC (5V)
- ESP32 to motors:
  GND - GND
  D1 - Controllr 1 Signal
  D6 - Contoroller 2 Signal
- Others:
  Relay NO - Vacumn Pump opposite
  Relay Com - 7.7V Battery red
  Motor's RED and BLACK are connected to 7.7V Batteries
?May I ask on if the wriing is correct and why are there 3 wires between escontroller and motor?
## Contro-Soup
- Hand-carved wooden side stick controller
- ESP32 XIAO C3
- MPU-6050
Unsure on the actual form yet, it will be decided once carving is finished.
# Other
Currenly I'm testing an esp32 with MPU-6050. I am flashing it with Arduino IDE. It would be great if we create separate modules for each hardware module such as separate full function for the MPU, each motor and controller, just mapping them together in the main singleton. Some idiomatic clean code like what I write in Golang.
