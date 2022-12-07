#ifndef __SETTINGS_STORAGE_H__
#define __SETTINGS_STORAGE_H__

#include <stdio.h>
#include <stdlib.h>


#include "pico/stdlib.h"
#include "hardware/flash.h" // for the flash erasing and writing
#include "hardware/sync.h" // for the interrupts

//ITS POSSIBLE TO REUSE THE CMD ENUM FROM THE WIFISETTINGS



#ifdef USE_WIFIINTERFACE_SETTINGS
#include "wifi_interface.h"
//#define SETTING_ENTRY wifi_interface::CMD_INDEX SETTINGS_ENTRY
#endif

//USED TO CHECK VALUE OF SETTING_ENTRY::INVALID AND  SETTING_ENTRY::LENGHT
//IF VALUES ARENT MATCHING => ERASE FLASH
#define FLASH_CHECK_VALUE_START 42
#define FLASH_CHECK_VALUE_END 24

class settings_storage{
public:
    
#ifdef USE_WIFIINTERFACE_SETTINGS
    //c++11 typedef
    using SETTING_ENTRY = wifi_interface::CMD_INDEX;
#else
    enum class SETTING_ENTRY{
        INVALID, // DONT DELETE USED FOR INIT CHECK
        TIME,
        BRIGHTNES,
        FACEPLATE,
        DISPLAYORIENTATION,
        LENGHT // DONT DELETE ALSO USED FOR INIT CHECK
    };
#endif
    
 
  

    
    static inline const int SETTINGS_COUNT = ((int)SETTING_ENTRY::LENGHT+1); //+1 we want to write to LENGHT too
    static inline const int REQUIRED_PAGES = ((sizeof(uint8_t)/FLASH_PAGE_SIZE)/SETTINGS_COUNT)+1; //ALIGN TO 256byte PAGES

    static inline const uint32_t FLASH_DATA_COUNT = FLASH_PAGE_SIZE * REQUIRED_PAGES; //FINAL REQUIRED SPACE IN 256byte * NEEDED PAGES

    static inline const int REQUIRED_SECTORS = (FLASH_DATA_COUNT/FLASH_SECTOR_SIZE)+1;
    static inline const uint32_t FLASH_SECTOR_COUNT = FLASH_SECTOR_SIZE * REQUIRED_SECTORS;


    static inline const uint32_t FLASH_TARGET_OFFSET = (1792*1024); //(PICO_FLASH_SIZE_BYTES - FLASH_DATA_COUNT); // (1792*1024) % 256 = 0
    static inline uint8_t storage_data[FLASH_DATA_COUNT]; // (FLASH_PAGE_SIZE)256 ALIGNMENT
    


    
    static void init();
    static uint8_t read(SETTING_ENTRY _entry);
    static bool write(SETTING_ENTRY _entry, uint8_t _value);


};

#endif

