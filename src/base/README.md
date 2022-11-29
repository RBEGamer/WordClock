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

```bash
$ cd ./wordclock
$ cmake .
$ make
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
See `wifi_interface.cpp` for bidirection reference implementation.

```
# <COMMAND>_<VLAUE>_<CRC (from command+value)>
```

### POSSIBLE COMMANDS

* `st` - set time with value: `hh:mm:ss`
* `sb` - set brightness: `1-255` fixed or `0` for automatic
* `fp` - frontplate: `0`(GERMAN), `1`(ENGLISH), ... (see `enum class FACEPLATES` in `wordclock_faceplate.hpp`)
* `fd` - flip displaz: rotate the display 180 drg `0`(normal, lightsensor top) or `1`
* `ip` - show ip string (used to indicate wifimodule ip) `192.168.0.1`


## DEBUGGING

All serial commands send through the serial pins `0`, `1` of the rp2040 will be mirrored to the USB-CDC connection.