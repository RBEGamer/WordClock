# BASE - MAIN RP2040 SOFTWARE

This folder contains the software for the rp2040 on the board.

* `wordclock` - main source
* `pico-sdk` - unmodified pico-sdk, used by `wordclock`
* `picotool` - picotool to flash `.u2f` file to pico (useful for mac)

Here all basic functionalities are implemented such as:

* Clockfaces: e.g. German, English, Binary, Dotted
* LED Matrix control
* Sensor interface
* wifimodule communication 



## BUILD

## FOR LATEST PCB VERSION
```bash
$ cd ./wordclock
$ cmake .
$ make
```

## SELECT BOARD: PCBv1

To select a different board configuration, thee the `wordclock/board/` folder.
This folder contatins a configuration for each PCB version.

To set the board, edit the `wordclock/CMakeLists.txt` and set `19 | set(PICO_BOARD "WORDCLOCK_V1_RP2040")` to the board filename without suffix.

### OR USING A COMMANDLINE SWITCH

```bash
$ cmake -DPICO_BOARD=WORDCLOCK_V1_RP2040 .
```

## FLASH

```
# RP2040 IN DFU MODE
$ ./picotool load ./wordclock/wordclock.u2f
$ ./picotool reboot
```





## SERIAL COMMANDS

These commands will be used by the wifi-module.
The basic command structure uses a key_value_crc approch.
See `./wordclock/src/wifi_interface.cpp` for bidirectional reference implementation.

```
# <COMMAND>_<VLAUE>_<CRC16 (from command+value with poly=0x8408)>
```

### POSSIBLE COMMANDS

* `st` - set time with value: `hh:mm:ss`
* `sb` - set brightness: `1-255` fixed or `0` for automatic
* `fp` - frontplate: `0`(GERMAN), `1`(ENGLISH), ... (see `enum class FACEPLATES` in `wordclock_faceplate.hpp`)
* `fd` - flip displaz: rotate the display 180 drg `0`(normal, lightsensor top) or `1`
* `ip` - show ip string (used to indicate wifimodule ip) `192.168.0.1`


## DEBUGGING

All serial commands send through the serial pins `0`, `1` of the rp2040 will be mirrored to the USB-CDC connection.
For advanced debugging use the SWD `RPSWD - P1` connector on PCBv2, to connect a Picoprobe or J-LINK.



## ADDING CUSTOM FACEPLATES

The current software implementation is very flexible in order to add new faceplates.
There are two template files provided with a simple implementation of a clockface.

* `./wordclock/src/wordclock_faceplate_template.hpp`
* `./wordclock/src/wordclock_faceplate_template.cpp`

Please see reference implementation for a word-base clockface:

* `./wordclock/src/wordclock_faceplate_german.hpp`
* `./wordclock/src/wordclock_faceplate_german.cpp`

If you dont have words on your fontface and wanto to implement other stlyes (like random dots, binary, for fancy animations), see the reference implementation for a dot based clockface:

* `./wordclock/src/wordclock_faceplate_dots.hpp`
* `./wordclock/src/wordclock_faceplate_dots.cpp`



