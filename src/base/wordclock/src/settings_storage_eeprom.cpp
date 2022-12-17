#include "settings_storage_eeprom.hpp"



settings_storage_eeprom::settings_storage_eeprom(const int _eeprom_i2c_addr){
    eeprom_i2c_addr = _eeprom_i2c_addr;
}

settings_storage_eeprom::~settings_storage_eeprom(){

}


void settings_storage_eeprom::restore_default()
{
    settings_storage::storage_data[(uint8_t)settings_storage::SETTING_ENTRY::INVALID] = settings_storage::FLASH_CHECK_VALUE_START;
    settings_storage::storage_data[(uint8_t)settings_storage::SETTING_ENTRY::LENGHT] = settings_storage::FLASH_CHECK_VALUE_END;

    settings_storage::storage_data[(uint8_t)settings_storage::SETTING_ENTRY::SET_FACEPLATE] = WORDCLOCK_LANGUAGE;
    settings_storage::storage_data[(uint8_t)settings_storage::SETTING_ENTRY::SET_DISPLAYORIENTATION] = WORDCLOCK_DISPlAY_ORIENTATION;
    settings_storage::storage_data[(uint8_t)settings_storage::SETTING_ENTRY::SET_BRIGHTNES] = WORDCLOC_BRIGHTNESS_MODE;
}

void settings_storage_eeprom::init()
{
    if (settings_storage_eeprom::read(settings_storage::SETTING_ENTRY::INVALID) != settings_storage::FLASH_CHECK_VALUE_START || settings_storage_eeprom::read(settings_storage::SETTING_ENTRY::LENGHT) != settings_storage::FLASH_CHECK_VALUE_END)
    {
        settings_storage_eeprom::restore_default(); 
    }
}

uint8_t settings_storage_eeprom::read(settings_storage::SETTING_ENTRY _entry)
{

    uint8_t buf_rx[1] = { (EEPROM_DATA_START_OFFSET + (uint8_t)_entry) };

    i2c_write_blocking(i2c_default, eeprom_i2c_addr, buf_rx, 1, true);
    const int num_bytes_read = i2c_read_blocking(i2c_default, eeprom_i2c_addr, buf_rx, 1, false);
    if(num_bytes_read > 0){
        return buf_rx[0];
    }
   return 0;
}

bool settings_storage_eeprom::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry == settings_storage::SETTING_ENTRY::INVALID || _entry == settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // STORE IN ARRAY
    settings_storage::storage_data[(uint8_t)_entry] = _value;
    // WRITE
    const uint8_t buf_tx[2] = {(uint8_t)(EEPROM_DATA_START_OFFSET + (uint8_t)_entry), _value};
    if(i2c_write_blocking(i2c_default, eeprom_i2c_addr, buf_tx, 2, true) == PICO_ERROR_GENERIC){
        return false;
    }
    return true;
}