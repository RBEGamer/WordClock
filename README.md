# Wordclock

![complete](./documentation/images/IMG_9906.png)

## FEATURES

Simple easy to build tabletop wordclock with many features:

* wifi base sync and control
* different clockfaces and languages
* RP2040 based
* ambient light sensor
* temperature sensor
* alternative datasources: mqtt, rest-api
* alternative modes: running text



## PARTS

### ELECTRICAL

* 1x ASSEMBLED PCB - `./src/pcb/vX`
* 1x MicroUSB to DIP breakout or USB Cable with open leads

### MECHANICAL

All needed drawings are located in the `./src/lasercut` directory.
The `drawing.svg` contatins every layer (as seperate object), so it can be seperated and used with K40Whisperer software for the K40 lasercutter.

* 1x - spacers - >220x220mm (approx 0.25m^2) >=3mm plywood (or plexiglas, but its hidden)
* 1x - plexiglas - >220x220 >=3mm, solid color for faceplate (with words cut in)
* 1x - plexiglas - opaque <30% transparent , >=3mm 


*See `./src/lasercut` for cutting instructions*

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





