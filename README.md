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
Set the bridges to `00001100`, connect the card via usb to your computer and run `pio run -t upload`.

Go to the `ifd-project` (`cd ..`) directory and run `pio run`.
Set the bridges to `11110000`, connect the card via usb to your computer and run `pio run -t upload`.

### Node-Red

This project includes a Node-Red flow, located at `flows_anarchy.json`.
You should copy that file into your node-red storage folder.

You can then run Node-Red using `npm`:

```sh
npm i
npm run start
```

The GUI should then be available at `http://localhost:1880/ui/`.

<!--
I sadly have no more information for you; node-red is a piece of software that involves the mouse to do stuff, and I don't abide by that.
If you can't figure this out (I wouldn't be able to either), then refer to the next section for the "API".
-->

## API

The card communicates via `MQTT` to a broker; its messages are in the following format:

```ebnf
(* This is EBNF. Check out https://fr.wikipedia.org/wiki/Extended_Backus-Naur_Form for more information! *)
msg = "[", command, [{":", value}], "]";
any_character = ? any character, bar ":" and "]" ?;
uppercase = ? uppercase letters ?;
command = {uppercase};
value = {any_character};
```

The commands used are the following:

- The card sends the `[QUERY]` message to the `TOPIC_OUT` channel; you may answer to such a message by sending it back the number of people using the `SET` message (see below)
- The card periodically sends the `UPDATE` command, with as parameters:
  - The current number of people in the room
  - The current arrival rate in humans per minute
  - The average number of people in the room
  - The average time spent in the room in seconds
  (eg. `[UPDATE:3:2.0:2.5:47]` for `3` people in the room, `2.0` people coming in per minute, `2.5` people in the room in average and `47` seconds spent in average in the room)
- The cards may receive a `SET` command, with as parameter the number of people that the card should think are in the room (eg. `[SET:3]` to set the counter to 3)
