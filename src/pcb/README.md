# PCB



## V1

The first revision of the wordclock pcb is working but has some issues!
Please see PCB version 2, with all fixed issues and further features.

### ISSUES

* r12 produces flash issues
* esp8266-01 creates space issues with usb connector
* levelshifter for ws2812 not working relilable with 800khz
* rtc not readlable for i2c, power issue => can be resolved by connecting VCC of the DS1307 chip to 5V.

### JUMPER SETTINGS

In order to make this version of the pcb compatible with the current firmware version, please use the following jumper settings:

* !!! REMOVE TRANSISTOR Q2 !!!
* !!! REMOVE RESISTOR R12 !!!
* bridge JP1
* bridge JP7 (GP16)
* jumper H23 (middle+upper, near JP2)


### WIFI-MODULE

If you want to use the ESP8266-01 you have to put a missing 10K (or bridge) the pins VCC and EN (CH-PD) on the module in order to enable the module.
This resistor is missing on the PCB.





## V2



### CHANGES

* replace ds1307 with pcf85263a
* remove R12 (resolve flash access issues)
* added esp8266-12f (replacement for esp8266-01)
* removed transistor base levelshifter (replaced with txb0101)
* added rpi rst pullup (optional)
* moved coincell to toplayer
* added loadcaps 10pf for 12mhz osc
* added rst/run to swd connector
* removed german wordclock faceplate from the back of the pcb
* added i2c eeprom to avoid flash as settings store
