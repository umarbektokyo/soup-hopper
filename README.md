# Soup-Hopper
**PRINT LINK**: https://drive.google.com/file/d/1DVYpBXSPw7tf-m13rQiefNHga5u4Mh4T/view?usp=drive_link
The world is ending on July 7th... and in the ending of the world, soup turns into a can! Imagine if this soup-hopper could ride around, shooting into people (and otherworldy creatures that are ending the world) with a powerful water gun.
![demo](assets/demo.png)
# BOM
## Soup-Hopper
- Aluminium Can
- LXN 18650 2000mAh - 7.4V 14.8Wh
- ESP32 XIAO C3
- MPU-6050
The main body is made of an aluminium can. On both sides, there is a motor (2207), an MPU-6050, ESP32 XIAO C3. Additionally, it will be able to sprink water, but that is a separate system for later. The goal with the base Soup-Hopper's form is to be able to ride forward/backward while the orientation of the robot stays the same. The controls are: turning right/left (wheels turn opposite dimensions) and also turning up/down (looking up and down whilst in-place idk how if we have only two motors).
![wiring](assets/wiring.png)
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
## Contro-Soup
- Hand-carved wooden side stick controller
- ESP32 XIAO C3
- MPU-6050
- Wiring was handled handled by @popo, we have this:
Wiring:
- Analog Stick: Y: GPIO PIN 0, X: GPIO PIN 1, Click: PIN 5
- Extra Buttons: Front: GPIO 4, Back-Up: GPIO 2, Back-Bottom: GPIO 3
- Accelerometer: SDL: 9, SDA: PIN 8
Unsure on the actual form yet, it will be decided once carving is finished.
# Other
Currenly I'm testing an esp32 with MPU-6050. I am flashing it with Arduino IDE. It would be great if we create separate modules for each hardware module such as separate full function for the MPU, each motor and controller, just mapping them together in the main singleton. Some idiomatic clean code like what I write in Golang.
We are communicating between the two esp32's on the controller and hopper through peer to peer ESP communication.
We must figure out if PID is necessary and how to implemented into our porject.
Whilst writing code, all of the constants must be defined at the beginning fo the file, making it easy to fine tune parameters.
# Controls
- Joystick forward/backward will correlate to the movement of the hopper (both wheels go front back I suppose.)
- Joystick right/left will correlate to the rotation of the robot (one wheel front, other back.)
- There is an IMU6050 inside of the robot, it would be great if we could make so the robot's main cylinder stay stable whilst the wheels rotate.
