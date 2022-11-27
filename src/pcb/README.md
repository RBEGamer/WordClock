# PCB



## V1

### ISSUES

* r12 produces flash issues
* esp8266-01 creates space issues with usb connector
* levelshifter for ws2812 not working relilable with 800khz
* rtc not readlable for i2c


## V2

### CHANGES

* replace ds1307 with pcf85263a
* remove R12 (resolve flash access issues)
* added esp8266-12f (replacement for esp8266-01)
* removed transistor base levelshifter (replaced with txb0101)
* added rpi rst pullup (optional)
* moved coincell to toplayer
