#ifndef __SETTINGS_STORAGE_H__
#define __SETTINGS_STORAGE_H__

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"

//ITS POSSIBLE TO REUSE THE CMD ENUM FROM THE WIFISETTINGS



#ifdef USE_WIFIINTERFACE_SETTINGS
#include "wifi_interface.h"
//#define SETTING_ENTRY wifi_interface::CMD_INDEX SETTINGS_ENTRY
#endif



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
    //USED TO CHECK VALUE OF SETTING_ENTRY::INVALID AND  SETTING_ENTRY::LENGHT
    //IF VALUES ARENT MATCHING => ERASE FLASH
    const int FLASH_CHECK_VALUE_START = 42;
    const int FLASH_CHECK_VALUE_END = 24;
    
    uint8_t storage_data[((int)SETTING_ENTRY::LENGHT)+1];


    settings_storage();
    virtual ~settings_storage();

    void restore_default();
    void init();
    uint8_t read(SETTING_ENTRY _entry);
    bool write(SETTING_ENTRY _entry, uint8_t _value);


};

#endif

