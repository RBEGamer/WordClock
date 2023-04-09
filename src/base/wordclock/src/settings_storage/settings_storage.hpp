#ifndef __SETTINGS_STORAGE_H__
#define __SETTINGS_STORAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <typeinfo>

#include "pico/stdlib.h"

#include "../helper.h"

// ITS POSSIBLE TO REUSE THE CMD ENUM FROM THE WIFISETTINGS
//USE CONFIG SETTINGS FROM WIFIINTERFACE, NEW ENTRIES MUST BE DECLARED IN WIFI INTERFACE
#define USE_WIFIINTERFACE_SETTINGS
#ifdef USE_WIFIINTERFACE_SETTINGS
#include "../wifi_interface.hpp"
// #define SETTING_ENTRY wifi_interface::CMD_INDEX SETTINGS_ENTRY
#endif

class settings_storage
{
public:
#ifdef USE_WIFIINTERFACE_SETTINGS
    // c++11 typedef
    using SETTING_ENTRY = wifi_interface::CMD_INDEX;
#else
    enum class SETTING_ENTRY
    {
        INVALID, // DONT DELETE
        LOG,
        TIME,
        BRIGHTNESS,
        DISPLAYIP,
        FACEPLATE,
        DISPLAYORIENTATION,
        BRIGHTNESSCURVE,
        DATE,
        DAYLIGHTSAVING,
        COLORMODE,
        RESTORESETTINGS,
        DOTBRIGHTNESS,
        BLINKENDOTS,
        LENGHT // DONT DELETE
    };
#endif

    settings_storage();
    virtual ~settings_storage();
    virtual void init();
    virtual void format();
    virtual uint8_t read(SETTING_ENTRY _entry);
    virtual bool write(SETTING_ENTRY _entry, uint8_t _value);

private:
    // THE BASECLaSS IMPLEMENTS A SIMPLE RAM BASED CONFIG STORAGE SYSTEM WHICH ISNT PERSISTANT
    //  FOR PERSISTANT STORAGE USE FLASH OR EEPROM BASED
    uint8_t storage_data_dummy[((int)SETTING_ENTRY::LENGHT) + 1];
};

#endif
