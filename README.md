# IFD Project: "The Wor Machine"

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

## Building and flashing

You will either need PIO installed as a VSCode extension or as a standalone extension.

### With the PIO VSCode extension

If you use the former, then you will need to add both `ifd-project` and `ifd-project/esp` as PIO projects.
Run "build" on both of them.

To flash the ESP build, set the bridges on the WeMos to `00001110` (5, 6 and 7 high).
Connect the card via usb to your computer, hope that it connected well and run "Upload".

To flash the ATMega build, set the bridges on the WeMos to `00110000` (1, 2, 3 and 4 high).
Connect the card via usb to your computer, hope that it connected well (it usually doesn't, try different cables) and run "Upload".

### With a standalone installation of PIO

Go to the `ifd-project/esp` (`cd esp/`) directory and run `pio run`.
Set the bridges to `00001110`, connect the card via usb to your computer and run `pio run -t upload`.

Go to the `ifd-project` (`cd ..`) directory and run `pio run`.
Set the bridges to `11110000`, connect the card via usb to your computer and run `pio run -t upload`.
