# This example lets you plug a BME280 breakout into your Pico Explorer and make a little indoor weather station, with barometer style descriptionsimport time
from breakout_bme280 import BreakoutBME280
from pimoroni_i2c import PimoroniI2C
from pimoroni import PICO_EXPLORER_I2C_PINS
from picographics import PicoGraphics, DISPLAY_PICO_EXPLORER
from array import *
import time

rtc = machine.RTC()
rtc.datetime((2022, 11, 11, 16, 13, 0, 0, 0))

# set up the hardware
display = PicoGraphics(display=DISPLAY_PICO_EXPLORER)
i2c = PimoroniI2C(**PICO_EXPLORER_I2C_PINS)


# lets set up some pen colours to make drawing easier

WHITE = display.create_pen(255, 255, 255)
BLACK = display.create_pen(0, 0, 0)
RED = display.create_pen(255, 0, 0)
GREY = display.create_pen(125, 125, 125)

M_ES = 0
M_IST = 1
M_FUENF = 2
M_ZEHN = 3
M_ZWANZIG = 4
M_VIERTEL = 5
M_VOR = 6
M_NACH = 7
M_HALB = 8
M_UHR = 21
H_EINS = 11
H_ZWEI = 12
H_DREI = 13
H_VIER = 14
H_FUENF = 10
H_SECHS = 15
H_SIEBEN = 17
H_ACHT = 16
H_NEUN= 9
H_ZEHN = 19
H_ELF = 20
H_ZWOELF = 18
H_EIN = 22

NUM_NEOPIXELS = 10*11
CLOCKWORDS = [
  [9,10,-1,-1,-1,-1,-1,-1,-1,-1], # es 0 X OK
  [5,6,7,-1,-1,-1,-1,-1,-1,-1],  # ist 1 X OK
  [0,1,2,3,-1,-1,-1,-1,-1,-1],  # fuenf 2 X OK
  [11,12,13,14,-1,-1,-1,-1,-1,-1],  # zehn 3 X OK
  [15,16,17,18,19,20,21,-1,-1,-1], #  zwanzig 4 X OK
  [22,23,24,25,26,27,28,-1,-1,-1],  # viertel 5 X OK
  [33,34,35,-1,-1,-1,-1,-1,-1,-1],  # vor 6 X OK
  [43,42,41,40,-1,-1,-1,-1,-1,-1],  # nach 8 X OK
  [54,53,52,51,-1,-1,-1,-1,-1,-1],  # halb 7 X OK
  [102,103,104,105,-1,-1,-1,-1,-1,-1],  # neun 9  X OK
  [44,45,46,47, -1,-1,-1,-1,-1,-1],  # fuenf 10 X OK
  [55,56,57,58,-1,-1,-1,-1,-1,-1],  # eins 11 X OK
  [65,64,63,62,-1,-1,-1,-1,-1,-1],  # zwei 12 X OK
  [73,74,75,76,-1,-1,-1,-1,-1,-1],  # drei 13 X OK
  [66,67,68,69,-1,-1,-1,-1,-1,-1],  # vier 14 X OK
  [77,78,79,80,81,-1,-1,-1,-1,-1],  # sechs 15 X OK
  [87,86,85,84,-1,-1,-1,-1,-1,-1],  # acht 16 X OK
  [93,94,95,96,97,98, -1,-1,-1,-1],  # sieben 17 X OK
  [88,89,90,91,92,-1,-1,-1,-1,-1], # zwölf 18 X OK
  [99,100,101,102,-1,-1,-1,-1,-1,-1],  # zehn 19 X OK
  [47,48,49,-1,-1,-1,-1,-1,-1,-1],  # elf 20 X OK
  [109,108,107,-1,-1,-1,-1,-1,-1, -1],  # uhr 21 X X
  [55,56,57,-1,-1,-1,-1,-1,-1,-1],  # eins 11 X OK
]

from array import *

def get_color(_hsvpos, _bright):
    _hsvpos = _hsvpos % 255
    _bright = abs(_bright)
    if _bright > 255:
        _bright = 255
        
    bs = _bright / 255.0
    r = 0.0
    g = 0.0
    b = 0.0
    
    _hsvpos = 255 - _hsvpos
    if _hsvpos < 85:
        r = (255 - _hsvpos * 3) * bs
        g = 0
        b = (_hsvpos * 3) * bs
    elif _hsvpos < 170:
        _hsvpos = _hsvpos - 85
        b = (255 - _hsvpos * 3) * bs
        r = 0
        g = (_hsvpos * 3) * bs
    else:
        _hsvpos = _hsvpos - 170
        g = (255 - _hsvpos * 3) * bs
        b = 0
        r = (_hsvpos * 3) * bs
        
    return int(r), int(g), int(b)


def display_words_row(_x, _y, _coloroffset, _words,_colors) -> int:
    
    fz = 3 # FONT SIZE
    fzmul = int(fz * 5.5) # DISTANCE BETWEEN WORDS
    i = 0
    x = _x
    for w in _words:
        
        if _colors[i] is not None and _colors[i] > 0:
            if _colors[i] == 2:
                print(get_color(_coloroffset, 255))
                r,g,b = get_color(_coloroffset, 255)
                display.set_pen(display.create_pen(r, g, b))
            elif _colors[i] == 3:
                r,g,b = get_color(_coloroffset* +100, 255)
                display.set_pen(display.create_pen(r, g, b))
            else:
                display.set_pen(WHITE)
            
        else:
            display.set_pen(GREY)
            
        i = i + 1
        display.text(w,x, _y, 100, fz)
        x = x + fzmul  * len(w)
    


def set_word_display(_index, _coloroffset):
    
    y = 10
    x = 30
    yinc = 22
    
    
    display_words_row(x, y, _coloroffset, ['es', 'k', 'ist', 'a', 'funf'], [(M_ES in _index)*1, 0, (M_IST in _index)*1, 0, (M_FUENF in _index)*2])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['zehn', 'zwanzig'], [(M_ZEHN in _index)*2, (M_ZWANZIG in _index)*2])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['drei', 'viertel'], [(M_VIERTEL in _index)*2, (M_VIERTEL in _index)*2])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['vor', 'funk','nach'], [(M_VOR in _index)*1, 0, (M_NACH in _index)*1])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['halb', 'a', 'el', 'f', 'unf'], [(M_HALB in _index)*2, 0, (H_ELF in _index)*3, (H_ELF in _index or H_FUENF in _index)*3 ,(H_FUENF in _index)*3])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['ein', 's', 'xam','zwei'], [(H_EIN in _index or H_EINS in _index)*3, (H_EINS in _index)*3, 0, (H_ZWEI in _index)*3])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['drei', 'auj','vier'], [(H_DREI in _index)*3, 0, (H_VIER in _index)*3])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['sechs', 'nl','acht'], [(H_SECHS in _index)*3, 0, (H_ACHT in _index)*3])
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['sieben', 'zwolf'], [(H_SIEBEN in _index)*3, (H_ZWOELF in _index)*3]) 
    y = y + yinc
    display_words_row(x, y, _coloroffset, ['zeh','n','eun', 'uhr'], [(H_ZEHN in _index)*3, (H_ZEHN in _index or H_NEUN in _index)*3, (H_NEUN in _index)*3, (M_UHR in _index)*1]) 
 
    display.update()

def clear_word_display():
    display.set_pen(BLACK)
    display.clear()
    
def time_to_words(_h, _m) -> []:
    
    word_array = [M_ES, M_IST]
    # ADD WORDS FOR HOURS
    if _h == 0 or _h == 12:
        word_array.append(H_ZWOELF)
    elif _h == 1 or _h == 13:
        if _m  >= 0 and _m < 5:
            word_array.append(H_EIN)
        else:
            word_array.append(H_EINS)
    elif _h == 2 or _h == 14:
        word_array.append(H_ZWEI)
    elif _h == 3 or _h == 15:
        word_array.append(H_DREI)
    elif _h == 4 or _h == 16:
        word_array.append(H_VIER)
    elif _h == 5 or _h == 17:
        word_array.append(H_FUENF)
    elif _h == 6 or _h == 18:
        word_array.append(H_SECHS)
    elif _h == 7 or _h == 19:
        word_array.append(H_SIEBEN)
    elif _h == 8 or _h == 20:
        word_array.append(H_ACHT)
    elif _h == 9 or _h == 21:
        word_array.append(H_NEUN)
    elif _h == 10 or _h == 22:
        word_array.append(H_ZEHN)
    elif _h == 11 or _h == 23:
        word_array.append(H_ELF)

    
    # ADD MIDDLE WORDS
    if _m >= 5 and _m < 25:
        word_array.append(M_NACH)
    if _m >= 35 and _m < 39:
        word_array.append(M_NACH)
    elif _m >= 25 and _m < 30:
        word_array.append(M_VOR)
    elif _m >= 40 and _m < 59:
        word_array.append(M_VOR)
    elif _m >= 0 and _m < 5:
        word_array.append(M_UHR)
        
    # ADD WORDS FOR MINUTES
    if _m >= 5 and _m < 10:
        word_array.append(M_FUENF)
    elif _m >= 10 and _m < 15:
        word_array.append(M_ZEHN)
    elif _m >= 15 and _m < 20:
        word_array.append(M_VIERTEL)
    elif _m >= 20 and _m < 25:
        word_array.append(M_ZWANZIG)
    elif _m >= 25 and _m < 30:
        word_array.append(M_FUENF)
        word_array.append(M_HALB)
    elif _m >= 30 and _m < 35:
        word_array.append(M_HALB)
    elif _m >= 35 and _m < 40:
        word_array.append(M_FUENF)
        word_array.append(M_HALB)
    elif _m >= 40 and _m < 45:
        word_array.append(M_ZWANZIG)
    elif _m >= 45 and _m < 50:
        word_array.append(M_VIERTEL)
    elif _m >= 50 and _m < 55:
        word_array.append(M_ZEHN)
    elif _m >= 55 and _m < 60:
        word_array.append(M_FUENF)
  
        
    return word_array




    
def display_time(_h, _m, _s):
    print(_h, _m, _s)
    clear_word_display()
    words = time_to_words(_h, _m)
    
    set_word_display(words, (_s * 4) % 255)

    
    
    



        
while True:

    display_time(rtc.datetime()[3], rtc.datetime()[4], rtc.datetime()[6])

    # waits for 1 second and clears to BLACK
    time.sleep(1)

