#include "settings_storage.hpp"

settings_storage::settings_storage(){
    
}

settings_storage::~settings_storage(){

}

void settings_storage::format(){

}

void settings_storage::restore_default()
{
    write(settings_storage::SETTING_ENTRY::INVALID, FLASH_CHECK_VALUE_START);
    write(settings_storage::SETTING_ENTRY::LENGHT, FLASH_CHECK_VALUE_END);
    write(settings_storage::SETTING_ENTRY::SET_FACEPLATE, WORDCLOCK_LANGUAGE);
    write(settings_storage::SETTING_ENTRY::SET_DISPLAYORIENTATION, WORDCLOCK_DISPlAY_ORIENTATION);
    write(settings_storage::SETTING_ENTRY::SET_BRIGHTNES, WORDCLOC_BRIGHTNESS_MODE);
}

void settings_storage::init()
{
    // CHECK IF THE SETTINGS ARE SET ONCE, ELSE SET THEM
    if (read(settings_storage::SETTING_ENTRY::INVALID) != settings_storage::FLASH_CHECK_VALUE_START || read(settings_storage::SETTING_ENTRY::LENGHT) != settings_storage::FLASH_CHECK_VALUE_END)
    {
        format();
        restore_default();
    }
}

uint8_t settings_storage::read(settings_storage::SETTING_ENTRY _entry)
{
    return settings_storage::storage_data_dummy[(int)_entry];
}

bool settings_storage::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry == settings_storage::SETTING_ENTRY::INVALID || _entry == settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // STORE IN ARRAY
    settings_storage::storage_data_dummy[(int)_entry] = _value;
    return true;
}