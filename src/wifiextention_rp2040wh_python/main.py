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
    final = _command + "_" + _payload + "_" + str(crc16(str.encode(str(_command)+str(_payload)))) + "\n"
    uart.write(final)
    print("send:", final)
    return final

def parse_cmd_str(_incomestr):
#    print("got:", _incomestr)
    if len(_incomestr) > 0 and "_" in _incomestr:
        sp = _incomestr.split("_")
#        print("sp:", sp)
        if len(sp) > 2:
            crc = str(crc16(str.encode(sp[0]+sp[1])))
            if str(crc) == str(sp[2]).split("\n")[0]: # DIRTY wAY TO REMOVE NEW line CHARAHTeR if present
                return sp[0], sp[1]
            else:
                print("crc mismatch", crc, sp[2])
    return None, ""
    
    
    
    

while True:
    
    
    # REC COMMANDS FROM WIFI MODULE
    rxData = bytes()
    rxData = bytes()
    if uart.any() > 0: 
        rxData += uart.readline()
        # If DATA READ
        if len(rxData) > 0:
            cmd, payload = parse_cmd_str(rxData.decode('utf-8'))
            print(cmd, "=>", payload)

        # to log
    send_cmd_str("st", "11:22:33")
    time.sleep(1)

