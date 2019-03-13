# Whistle Driven Robot

This project was developed for the course _Advanced Operating
Systems_ in the academic year 2017/2018 on the Politecnico of Milan.

#### Purpose

This project consists in a robot controlled by a whistle
that can be used to play some simple games, as following a path
or making some races. It is composed by two parts:
- The robot: realized through an Arduino Nano and 3D printed;
- The transmitter: an STM32F407 Discovery that recognizes
the frequency of the sound and sends the oppotune command
to the robot using a Bluetooth module.

#### Used libraries
- STM32 OS Miosix: https://miosix.org/
- Lorenzo Binosi and Matheus Fim’s Guitar Tuner Project: 
https://github.com/LorenzoBinosi/AOSPro ject?files=1