#ifndef __SETTINGS_STORAGE_EEPROM_H__
#define __SETTINGS_STORAGE_EEPROM_H__

#include <stdio.h>
#include <stdlib.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"


#include "settings_storage.hpp"

class settings_storage_eeprom: public settings_storage{
public:
    

    
 
  

    int eeprom_i2c_addr = 0x50;
    settings_storage_eeprom(const int _eeprom_i2c_addr);
    ~settings_storage_eeprom();
    void set_eeprom_i2c_addr(const int _eeprom_i2c_addr);
    void restore_default();
    void init();
    uint8_t read(SETTING_ENTRY _entry);
    bool write(SETTING_ENTRY _entry, uint8_t _value);


};

#endif

