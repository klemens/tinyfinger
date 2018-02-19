# TinyFinger

TinyFinger is a small device powered by an ATtiny45 to produce pseudorandom
numbers for selecting student groups during the computer science hardware
courses at Leipzig University.

## board

The board is designed in KiCad and uses a single sided pcb layout using
mostly SMD components. The routing was done manually.

## code

The firmware for the ATtiny is written as an Arduino sketch using the
[ATTinyCore] board definitions. It uses a [linear congruential generator]
for generating pseudorandom numbers and transforms them to the needed
range using the gcc uniform_int_dist c++std implementation.

Flashing the firmware can be done using a normal Arduino programmes as an
ISP with the ArduinoISP example sketch included with the IDE. The PINs on
the board are from left to right: GND, MISO, VCC, SCK, MOSI, RESET.

## case

The case is modelled in Blender and intended for 3d printing, e.g. using
Cura after exporting the model as an stl file.

[ATTinyCore]: https://github.com/SpenceKonde/ATTinyCore
[linear congruential generator]: https://en.wikipedia.org/wiki/Linear_congruential_generator

