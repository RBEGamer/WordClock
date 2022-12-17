#ifndef __SETTINGS_STORAGE_FLASH_H__
#define __SETTINGS_STORAGE_FLASH_H__

#include <stdio.h>
#include <stdlib.h>


#include "pico/stdlib.h"
#include "hardware/flash.h" // for the flash erasing and writing
#include "hardware/sync.h" // for the interrupts

#include "settings_storage.hpp"

class settings_storage_flash: public settings_storage{
public:
    

    
 
  

    
    static inline const int SETTINGS_COUNT = ((int)SETTING_ENTRY::LENGHT+1); //+1 we want to write to LENGHT too
    static inline const int REQUIRED_PAGES = ((sizeof(uint8_t)/FLASH_PAGE_SIZE)/SETTINGS_COUNT)+1; //ALIGN TO 256byte PAGES
    static inline const uint32_t FLASH_DATA_COUNT = FLASH_PAGE_SIZE * REQUIRED_PAGES; //FINAL REQUIRED SPACE IN 256byte * NEEDED PAGES
    static inline const int REQUIRED_SECTORS = (FLASH_DATA_COUNT/FLASH_SECTOR_SIZE)+1;
    static inline const uint32_t FLASH_SECTOR_COUNT = FLASH_SECTOR_SIZE * REQUIRED_SECTORS;
    static inline const uint32_t FLASH_TARGET_OFFSET = (1792*1024); //(PICO_FLASH_SIZE_BYTES - FLASH_DATA_COUNT); // (1792*1024) % 256 = 0
   
    
    settings_storage_flash();
    ~settings_storage_flash();

    void restore_default();
    void init();
    void format_flash();
    uint8_t read(SETTING_ENTRY _entry);
    bool write(SETTING_ENTRY _entry, uint8_t _value);


};

#endif

