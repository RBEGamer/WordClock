# LASERCUT



## V2

the V2 lasercutting files are loacated in the `./v2` folder.
The main source files is the single `drawing.svg` file, which contains several numbered layer object in it.
Each layer object in the svg will be used to cut one piece for the clock.
Some layers are needed multible times and will be glued together to form a higher single layer.

### FACEPLATE (PLEXIGLAS)

![4_digit_complete_alt](../../documentation/images/IMG_9894.jpg)

* 1x - `0_opaque_layer` - this is the semi-transparent toplayer plexiglas
* 1x - `1_text_<LANGUAGE>` - this is the textlayer with the cut out characters

 Clean (isopropanol), place and clamp! the `0_opaque_layer` and `1_text` layer ontop of each other and use the `ACRIFIX Plexiglas glue` to fill the minute dot holes with the glue and let it dry for at least 8h.

### LIGHTGUIDE (WOOD)

* 3x - `2_lightguide` - its use to get some space between the leds and the text faceplate

Glue two of the three sheets together and before adding the third one add one sheet of sandwich paper/ semi transparent diffusor between them.
In the end this additional diffusor sit ontop of the leds with one lightguide seperating them.


### BACKCOVER

* 1x - `3_backcover_with_mangets` - this is placed on the back of the pcb and has cutouts to place mangets and to access all connectors
* 1x - `3_backcover_without_mangets`



