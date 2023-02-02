# Wordclock

![complete](./documentation/images/IMG_9906.png)

![complete](./documentation/images/click_it_en_de_clockfaces.JPG)

## FEATURES

Simple easy to build tabletop wordclock with many features:


* different clockfaces and languages
* four colormodes (coldwhite, warmwhite, 2x rainbow)
* RP2040 based
* ambient light sensor
* temperature sensor
* eu summertime support
* optional wifi based sync and control

## LANGUAGES

* GERMAN - TESTED
* ENGLISH - TESTED
* ITALIAN - TESTING NEEDED

### POSSIBLE LANGUAGES

* SPAIN
* DUTCH




## MANUAL




### CONFIGURATION


After configuring the wifi, you can access the clock-configuration webpage using its DHCP address (see your router or nmap) or use the clocks mDNS adress `http://wordclock_%%ID%%__.local` with `__%%ID%%__` is the ID printed on the back of the clock, like: `http://wordclock10144907.local`.

**NOTE** After a wifi connection is established (power up, power cycle), the clock shows it on its display by means of the words. here the ip blocks are shown with the hours and the dot by using dark switching 0 or 12.

So `192.168.178.4` results in the following word order:
`one nine two  (pause)(dark 0/12)(pause) one six eight (pause)(dark 0/12)(pause) one seven eight (pause)(dark 0/12)(pause) four`









## PARTS

### ELECTRICAL

* 1x ASSEMBLED PCB - `./src/pcb/vX`
* 1x MicroUSB to DIP breakout or USB Cable with open leads

### MECHANICAL

All needed drawings are located in the `./src/lasercut` directory.
The `drawing.svg` contatins every layer (as seperate object), so it can be seperated and used with K40Whisperer software for the K40 lasercutter.

* 1x - spacers - >220x220mm (approx 0.25m^2) >=3mm plywood (or plexiglas, but its hidden)
* 1x - plexiglas - >220x220mm 1mm, solid color for faceplate (with words cut in)


**See `./src/lasercut` for cutting instructions**

#### OPTIONAL
* 8x neodym magnets - 10x3mm (if you want to make the faceplate removable/rotatable)


### OTHER

* sandwich paper - as additional semi transparent diffusor ontop of the leds
* woodglue - to glue spacers together
* ACRIFIX Plexiglas glue - to glue faceplate and opaque frontplate together
* isopropanol - plexiglas cleaning before gluing
### TOOLS

* lasercutter / cnc for cutting wood / plexiglas with working-area of at least 220mmx220mm
* 3d rpinter can be used to print the faceplates and spacers(conversion from svg-> 3mm stl is needed)




## STATUS CODE

Directly after startup, the clock shows some version information about the hardware revision and used software.


![complete](./documentation/status_codes.jpg)

## COLOR

