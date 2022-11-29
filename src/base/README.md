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