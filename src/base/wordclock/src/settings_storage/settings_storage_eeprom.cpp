#include "settings_storage_eeprom.hpp"

settings_storage_eeprom::settings_storage_eeprom(const int _eeprom_i2c_addr)
{
    set_eeprom_i2c_addr(_eeprom_i2c_addr);
    init();
}

settings_storage_eeprom::~settings_storage_eeprom()
{
}


void settings_storage_eeprom::format(){

}

void settings_storage_eeprom::init()
{
}


void settings_storage_eeprom::set_eeprom_i2c_addr(const int _eeprom_i2c_addr)
{
    settings_storage_eeprom::eeprom_i2c_addr = _eeprom_i2c_addr;
}

uint8_t settings_storage_eeprom::read(settings_storage::SETTING_ENTRY _entry)
{
    const uint8_t buf_rx[1] = {(uint8_t)_entry};
    uint8_t buf_rx_back[1] = {0};
    i2c_write_blocking(i2c_default, settings_storage_eeprom::eeprom_i2c_addr, buf_rx, 1, true);
    const int num_bytes_read = i2c_read_blocking(i2c_default, settings_storage_eeprom::eeprom_i2c_addr, buf_rx_back, 1, false);
    if (num_bytes_read > 0)
    {
        return buf_rx_back[0];
    }
    return 0;
}

bool settings_storage_eeprom::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry < settings_storage::SETTING_ENTRY::INVALID || _entry > settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // WRITE
    const uint8_t buf_tx[2] = {(uint8_t)_entry, _value};
    if (i2c_write_blocking(i2c_default, settings_storage_eeprom::eeprom_i2c_addr, buf_tx, 2, true) == PICO_ERROR_GENERIC)
    {
        return false;
    }
    return true;
}