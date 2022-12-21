#include "settings_storage.hpp"

settings_storage::settings_storage()
{
}

settings_storage::~settings_storage()
{
}

void settings_storage::format()
{
    for (size_t i = (size_t)settings_storage::SETTING_ENTRY::INVALID; i < (size_t)settings_storage::SETTING_ENTRY::LENGHT+1; i++)
    {
        settings_storage::storage_data_dummy[(int)i] = 0;
    }
}

void settings_storage::init()
{
    format();
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
