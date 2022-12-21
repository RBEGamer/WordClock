#include "settings_storage_eeprom.hpp"

settings_storage_eeprom::settings_storage_eeprom()
{
    settings_storage_eeprom::init();
}

settings_storage_eeprom::~settings_storage_eeprom()
{
}

void settings_storage_eeprom::format()
{
    for (size_t i = (size_t)settings_storage::SETTING_ENTRY::INVALID; i < (size_t)settings_storage::SETTING_ENTRY::LENGHT+1; i++)
    {
       settings_storage_eeprom::write((settings_storage::SETTING_ENTRY)i, 0);
    }
}

void settings_storage_eeprom::init()
{
}


uint8_t settings_storage_eeprom::read(settings_storage::SETTING_ENTRY _entry)
{
    uint8_t buf_rx[1] = {0};
    helper::reg_read(i2c_default, M24C02_I2C_ADDR, EEPROM_WRITE_OFFSET+ (uint8_t)_entry, buf_rx, 1);
    return buf_rx[0];
}

bool settings_storage_eeprom::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry < settings_storage::SETTING_ENTRY::INVALID || _entry > settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // WRITE
    uint8_t buf_tx[1] = {_value};
    return helper::reg_write(i2c_default, M24C02_I2C_ADDR, EEPROM_WRITE_OFFSET+ (uint8_t)_entry, buf_tx, 1);
}


