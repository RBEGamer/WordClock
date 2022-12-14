#ifndef __SETTINGS_STORAGE_EEPROM_H__
#define __SETTINGS_STORAGE_EEPROM_H__

#include <stdio.h>
#include <stdlib.h>


#include "pico/stdlib.h"
#include "settings_storage.hpp"

class settings_storage_eeprom: public settings_storage{
public:
    

    
 
  
private:
    const uint8_t EEPROM_WRITE_OFFSET = 0x01; //START USERDATA LATER IF WANTED

public:
    settings_storage_eeprom();
    ~settings_storage_eeprom();
    
    void init() override;
    void format() override;
    uint8_t read(SETTING_ENTRY _entry) override;
    bool write(SETTING_ENTRY _entry, uint8_t _value) override;

   

};

#endif

