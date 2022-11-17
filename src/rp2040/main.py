# WORDCLOCK PR2040 SOFTWARE
# FOR PCB V1 WITH OPTIONAL WIFI EXTENTION EXPORT
# CHANGELOG
# * added fakertc -> rtc support missing
# * cmd parser / sender working
#from picographics import PicoGraphics, DISPLAY_PICO_EXPLORER
import array, time
from machine import ADC
from array import *
import rp2

import machine
from machine import *
# UART ON GPIO0-TX AND GPIO1-RX TO COMMUNICATE$ WITH THE WIFI EXTENTION
uart = machine.UART(0, baudrate = 9600)
print("UART Info : ", uart)
# TO USE I"C PINS 2/3 ON THE RP2040 I2C BUS 1 HAS TO BE USED
i2c = machine.I2C(1, scl=machine.Pin(3), sda=machine.Pin(2))
print("I2C Info : ", i2c)


BH150_ADDR = 0x23
DS1307_ADDR = 0x68
bh150_enabled = False
ds1307_enabled = False
devices = i2c.scan()
print("# I2C DEVICES FOUND:")
if devices:
    for d in devices:
        print(hex(d))
        if d == BH150_ADDR:
            bh150_enabled = True
        elif d == DS1307_ADDR:
            ds1307_enabled = True

# INIT RTC
#rtc = machine.RTC()
# set up the hardware
#display = PicoGraphics(display=DISPLAY_PICO_EXPLORER)


# TIME VARIAbLES FOR FAKE RTC
time_h = 0
time_m = 0
time_s = 0
display_update_trigger = 0
display_set_brightness = 0 # 0=AUTO 1-255 FIXED
display_calc_brightness = 255 #



# lets set up some pen colours to make drawing easier

#WHITE = display.create_pen(255, 255, 255)
#BLACK = display.create_pen(0, 0, 0)
#GREY = display.create_pen(125, 125, 125)


# CLOCKWORDS INDEX
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


PIN_NEOPIXELS = 22
NUM_NEOPIXELS = (10*11)+4 # 4 minute leds
CLOCKWORDS = [
  [1,2,-1,-1,-1,-1,-1,-1,-1,-1], # es 0 X OK
  [4,5,6,-1,-1,-1,-1,-1,-1,-1],  # ist 1 X OK
  [7,8,9,10,11,-1,-1,-1,-1,-1],  # fuenf 2 X OK
  [23,22,21,20,-1,-1,-1,-1,-1,-1],  # zehn 3 X OK
  [13,14,15,16,17,18,19,-1,-1,-1], #  zwanzig 4 X OK
  [28,29,30,31,32,33,34,-1,-1,-1],  # viertel 5 X OK
  [45,44,43,-1,-1,-1,-1,-1,-1,-1],  # vor 6 X OK
  [35,36,37,38,-1,-1,-1,-1,-1,-1],  # nach 8 X OK
  [46,47,48,49,-1,-1,-1,-1,-1,-1],  # halb 7 X OK
  [106,107,108,109,-1,-1,-1,-1,-1,-1],  # neun 9  X OK
  [53,54,55,56,-1,-1,-1,-1,-1,-1],  # fuenf 10 X OK
  [67,66,65,64,-1,-1,-1,-1,-1,-1],  # eins 11 X OK
  [57,58,59,60,-1,-1,-1,-1,-1,-1],  # zwei 12 X OK
  [68,69,70,71,-1,-1,-1,-1,-1,-1],  # drei 13 X OK
  [78,77,76,75,-1,-1,-1,-1,-1,-1],  # vier 14 X OK
  [89,88,87,86,85,-1,-1,-1,-1,-1],  # sechs 15 X OK
  [79,80,81,82,-1,-1,-1,-1,-1,-1],  # acht 16 X OK
  [90,91,92,93,94,95,-1,-1,-1,-1],  # sieben 17 X OK
  [96,97,98,99,100,-1,-1,-1,-1,-1], # zwölf 18 X OK
  [112,111,110,109,-1,-1,-1,-1,-1,-1],  # zehn 19 X OK
  [51,52,53,-1,-1,-1,-1,-1,-1,-1],  # elf 20 X OK
  [102,103,104,-1,-1,-1,-1,-1,-1,-1],  # uhr 21 X X
  [67,66,65,-1,-1,-1,-1,-1,-1,-1],  # eins 11 X OK
]

@rp2.asm_pio(sideset_init=rp2.PIO.OUT_LOW, out_shiftdir=rp2.PIO.SHIFT_LEFT, autopull=True, pull_thresh=24)
def ws2812():
    T1 = 2
    T2 = 5
    T3 = 3
    wrap_target()
    label("bitloop")
    out(x, 1)               .side(0)    [T3 - 1]
    jmp(not_x, "do_zero")   .side(1)    [T1 - 1]
    jmp("bitloop")          .side(1)    [T2 - 1]
    label("do_zero")
    nop()                   .side(0)    [T2 - 1]
    wrap()


# Create the StateMachine with the ws2812 program, outputting on pin
sm = rp2.StateMachine(0, ws2812, freq=8_000_000, sideset_base=Pin(PIN_NEOPIXELS))
# Start the StateMachine, it will wait for data on its FIFO.
sm.active(1)
# Display a pattern on the LEDs via an array of LED RGB values.
#ar = array.array("I", [0 for _ in range(NUM_NEOPIXELS)])
pixel_array = array("I", [0 for _ in range(NUM_NEOPIXELS)])


def temperature():
    temp_sensor = ADC(4)
    raw_sensor_data = temp_sensor.read_u16()
    sensor_voltage = (raw_sensor_data / 65535)*3.3
    temperature = 27. - (sensor_voltage - 0.706)/0.001721
    return temperature


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
        
    return (int(r), int(g), int(b))

def update_pix(): # dimming colors and updating state machine (state_mach)
    dimmer_array = array("I", [0 for _ in range(NUM_NEOPIXELS)])
    for ii,cc in enumerate(pixel_array):
        r = int(((cc >> 8) & 0xFF)) # 8-bit red dimmed to brightness
        g = int(((cc >> 16) & 0xFF)) # 8-bit green dimmed to brightness
        b = int((cc & 0xFF)) # 8-bit blue dimmed to brightness
        dimmer_array[ii] = (g<<16) + (r<<8) + b # 24-bit color dimmed to brightness
    sm.put(dimmer_array, 8) # update the state machine with new colors
    time.sleep_ms(10)

def set_24bit(ii, color): # set colors to 24-bit format inside pixel_array
    pixel_array[ii] = (color[1]<<16) + (color[0]<<8) + color[2] # set 24-bit color
    


def set_word_display(_words, _coloroffset, _brgh):
    # TEXT POSITION ON DISPLAY LEFT/TOP CORNER
    
    
 
    penm = get_color(_coloroffset, _brgh)
    penh = get_color(_coloroffset+100, _brgh)
    for i in range(NUM_NEOPIXELS):
        set_24bit(i, (0, 0, 0))
        
    for word in _words:
        if word >= 0:
            for led in CLOCKWORDS[word]:
                if led >= 0:
                    set_24bit(led, penm)
                else:
                    set_24bit(led, (0,0,0))

        
    update_pix()
        


def clear_word_display():
    for i in range(NUM_NEOPIXELS):
        set_24bit(i, (0, 0, 0))
    update_pix()
    
def time_to_words(_h, _m) -> []:
    # ARRAY 
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
    if _m >= 35 and _m < 40:
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


def crc16(data: bytes, poly=0x8408):  
    data = bytearray(data)
    crc = 0xFFFF
    for b in data:
        cur_byte = 0xFF & b
        for _ in range(0, 8):
            if (crc & 0x0001) ^ (cur_byte & 0x0001):
                crc = (crc >> 1) ^ poly
            else:
                crc >>= 1
            cur_byte >>= 1
    crc = (~crc & 0xFFFF)
    crc = (crc << 8) | ((crc >> 8) & 0xFF)
    return crc & 0xFFFF

    
def send_cmd_str(_command, _payload): 
    final = str(_command) + "_" + str(_payload) + "_" + str(crc16(str.encode(str(_command)+str(_payload)))) + "\n"
    uart.write(final)
    print("send:", final)
    return final

def parse_cmd_str(_incomestr):
    #print("got:", _incomestr)
    if len(_incomestr) > 0 and "_" in _incomestr:
        sp = _incomestr.split("_")
        #print("sp:", sp)
        if len(sp) > 2:
            crc = str(crc16(str.encode(sp[0]+sp[1])))
            if str(crc) == str(sp[2]).split("\n")[0]: # DIRTY wAY TO REMOVE NEW line CHARAHTeR if present
                return sp[0], sp[1]
            else:
                print("crc mismatch", crc, sp[2])
    return None, ""
    
    
    
    
def display_time(_h, _m, _s, _brgh):
    send_cmd_str("ct", "{0}:{1}:{2}".format(_h, _m, _s))
    words = time_to_words(_h, _m)
    set_word_display(words, (_s * 4) % 255, _brgh)

    
    
def init_bh1750(i2c_addr = 0x23):
    i2c.writeto_mem(i2c_addr, 0x00, bytearray([0x00])) # OFF
    i2c.writeto_mem(i2c_addr, 0x01, bytearray([0x01])) # ON
    i2c.writeto_mem(i2c_addr, 0x07, bytearray([0x07])) # RESET
    # SET MODE
    i2c.writeto_mem(i2c_addr, 0x13, bytearray([0x13])) # CONT_LOWRES

    
def read_bh1750(i2c_addr = 0x23, _default_brght = 255, _min_brght = 10):
    print("read_bh1750")
    data = i2c.readfrom(i2c_addr, 2) # READ 2 BYTES
    factor = 2.0
    res = (data[0]<<8 | data[1]) / (1.2 * factor)
    
    
    # 505 LUX = BRIGHT LIVINGROOM SENSOR DELIVERS 1-65536 LUX SO MAP 0-505 => 255-_min_brght
    return max(_min_brght, _default_brght, min(255,min(res,505)/2))


def rtc_init():
    if ds1307_enabled:
        pass
    else:
       rtc_settime(0, 0, 0)

def rtc_gettime():
    # rtc.datetime()[3], rtc.datetime()[4], rtc.datetime()[6]
    if ds1307_enabled:
        pass
    else:
        global time_s
        global time_m
        global time_h
        return time_h, time_m, time_s

def rtc_settime(_h, _m, _s):
    if ds1307_enabled:
        pass
    else:
        global time_s
        global time_m
        global time_h
        time_h = _h
        time_m = _m
        time_s = _s


def rtc_update():
    if ds1307_enabled:
        pass
    else:
        global time_s
        global time_m
        global time_h
        time_s = time_s + 1    
        if time_s >= 60:
           time_s = 0
           time_m = time_m +1
        if time_m >= 60:
           time_m = 0
           time_h = time_h +1
        if time_h >= 24:
           time_h = 0





if bh150_enabled:
    init_bh1750()
    send_cmd_str("bh1750", "enabled")
else:
    send_cmd_str("bh170", "disabled")

rtc_init()
if ds1307_enabled:
    send_cmd_str("ds1307", "enabled")
else:
    send_cmd_str("ds1307", "disabled")
    
clear_word_display()
while True:
    
    
    # REC COMMANDS FROM WIFI MODULE
    rxData = bytes()
    if uart.any() > 0: 
        rxData += uart.readline()
        # If DATA READ
        if len(rxData) > 0:
            cmd, payload = parse_cmd_str(rxData.decode('utf-8'))
        # SET TIME CMD
            if cmd is not None and cmd == "st" and ":" in payload:
                sp = payload.split(":")
                if len(sp) > 2:
                    rtc_settime(int(sp[0]), int(sp[1]), int(sp[1]))
            # SET BRIGHTNESS CMD
            elif cmd is not None and cmd == "sb" and len(payload) >= 0:
                display_set_brightness = max(min(int(payload),255),0)
            #send_cmd_str("log", str(cmd) + ":" + str(payload))
        
        
    # UPDATE DISPLAY EVERY X CYCLES
    if display_update_trigger > 10:
        # CALC BRIGHTNESS
        if display_set_brightness == 0:
            if bh150_enabled:
                display_calc_brightness = read_bh1750(display_set_brightness)
                send_cmd_str("cb", display_calc_brightness)
            else:
                display_calc_brightness = 255
        else:
            display_calc_brightness = display_set_brightness
            send_cmd_str("cb", display_calc_brightness)
            
        # READ TEMP
        #send_cmd_str("temp", str(int(temperature()*100)/100.0))
        
        # UPDATE DISPLAY
        rtc_update()
        h, m, s = rtc_gettime()
        display_update_trigger = 0
        display_time(h, m, s, display_calc_brightness)
    display_update_trigger += 1
    #print(temperature())
    # waits for 1 second and clears to BLACK
    time.sleep(0.1)



