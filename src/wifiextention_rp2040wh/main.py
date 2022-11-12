# WORDCLOCK PR2040 SOFTWARE
# FOR PCB V1 WITH OPTIONAL WIFI EXTENTION EXPORT


from array import *
import time
from array import *

import network
if hasattr(network, "WLAN"):
   print("WH")

# UART ON GPIO0-TX AND GPIO1-RX TO COMMUNICATE$ WITH THE WIFI EXTENTION
uart = machine.UART(0, baudrate = 9600)
print("UART Info : ", uart)

def crc16(data: bytes, poly=0x8408):
    '''
    CRC-16-CCITT Algorithm
    '''
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
    final = _command + "_" + _payload + "_" + str(crc16(str.encode(_command+_payload)) + "\n")
    uart.write(final)
    print("send:", final)
    return final

def parse_cmd_str(_incomestr):
    print(":", _incomestr)
    if len(_incomestr) > 0 and "_" in _incomestr:
        sp = _incomestr.split("_")
        if len(sp) > 2:
            crc = str(crc16(str.encode(sp[0]+sp[1])))
            if crc == sp[2]:
                return sp[0], sp[1]
            else:
                print("crc mismatch", crc, sp[2])
    return None, ""
    
    
    
    

    
 #   send_cmd_str("ct", "{0}:{1}:{2}".format(_h, _m, _s))



while True:
    
    
    # REC COMMANDS FROM WIFI MODULE
    rxData = bytes()
    while uart.any() > 0:
        rxData += uart.read(1)
    if len(rxData) > 0:
        cmd, payload = parse_cmd_str(rxData.decode('utf-8'))
        # to log

    time.sleep(0.2)

