# UVLED-timerControl
LED timer using Serial Monitor from Arduino

Using an Arduino UNO I wish to control LED 'on' time. The purpose is an experimental setup in which UV irradiation is applied to specific samples. 

The UV LED is a LZ1-30UV00 from LED-engin (365nm). The current to the LED is regulated with a LM350 positive adjustment regulator, allowing any power supply between 9V and 35V to be used. In the current setup the power supply is a simple lab-supply, but this can be replaced by any power supply able to deliver 800 mA with the voltage between 9 - 35V.

The Arduino code is assembled from two sources:
1. millisDelay from Forward Computing and Control Pty. Ltd. NSW Australia (www.forward.com.au)

2. For the serial monitor reading I use the examples found on the Arduino forum:
http://forum.arduino.cc/index.php?topic=396450 (retreived 20 November 2018) written by a forum member called Robin2

Components - UV LED:
- 365nm LED				(LZ1-30UV0000 LED Engin, mini-round MCPCB)
- LM350 positive adjustment regulator	(Fairchild)
- 1.6 Ohm 2W resistor
- Heatsink

Components - Powerswitch:
- N-Channel MOSFET			(FQP20N06L, Fairchild)
- 220 Ohm resistor
- 10 kOhm resistor
- Two small heat sinks

Components - Other
- Arduino UNO 
- Laptop
- Lab DC power supply 			(Delta elektronika)