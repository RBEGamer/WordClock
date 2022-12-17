#include "settings_storage.hpp"

settings_storage::settings_storage(){
    
}

settings_storage::~settings_storage(){

}


void settings_storage::restore_default()
{
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::INVALID] = FLASH_CHECK_VALUE_START;
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::LENGHT] = FLASH_CHECK_VALUE_END;

    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_FACEPLATE] = WORDCLOCK_LANGUAGE;
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_DISPLAYORIENTATION] = WORDCLOCK_DISPlAY_ORIENTATION;
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_BRIGHTNES] = WORDCLOC_BRIGHTNESS_MODE;
}

void settings_storage::init()
{
    if (settings_storage::read(settings_storage::SETTING_ENTRY::INVALID) != FLASH_CHECK_VALUE_START || settings_storage::read(settings_storage::SETTING_ENTRY::LENGHT) != FLASH_CHECK_VALUE_END)
    {
        printf("settings_storage erase data due checksum mismatch");
        restore_default();
    }
}

uint8_t settings_storage::read(settings_storage::SETTING_ENTRY _entry)
{
    return settings_storage::storage_data[(int)_entry];
}

bool settings_storage::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry == settings_storage::SETTING_ENTRY::INVALID || _entry == settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // STORE IN ARRAY
    settings_storage::storage_data[(int)_entry] = _value;
    return true;
}