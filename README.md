# IFD Project

This repository contains the necessary bits of code and schematics to reproduce our group's project for the "IFD" (Practical Informatics) class.
Our group's subject is a counter for the number of people present in a store or place.
It uses [Little's Law](https://en.wikipedia.org/wiki/Little's_law) to determine the average time spent by people in the shop; the necessary data is sent through MQTT to do the calculations and have a means of permanent memory.
The device also measures the temperature and buzzes if it is abnormally high.

The materials needed for it is:

- 1 ATMega2560, equipped with an `esp01_1m`-like chip (we used a WeMOS card for that matter)
- 2 light sensors
- 2 laser emitters
- 1 temperature sensor
- 1 buzzer
- 1 button

## Installation

First, download this repository and set initially set up the credentials:

```sh
git clone https://github.com/adri326/ifd-project
cd ifd-project
cp esp/src/credentials.cpp.sample esp/src/credentials.cpp
# You will need to edit esp/src/credentials.cpp with the correct settings
```

You will then either need PIO installed as a VSCode extension or as a standalone extension.
If you use the former, then you will need to add both `ifd-project` and `ifd-project/esp` to the PIO gui, and tell it to build and flash them to resp. the ATMega and the ESP chip.
If you use the latter, then all you need to do is run `pio run` to build the project and `pio run -t upload` to flash resp. the ATMega and the ESP chip.

<!-- Gonna fix that documentation, I got a bus to take. -->
