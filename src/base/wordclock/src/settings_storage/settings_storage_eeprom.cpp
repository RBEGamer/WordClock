#include "settings_storage_eeprom.hpp"

settings_storage_eeprom::settings_storage_eeprom(const int _eeprom_i2c_addr)
{
    settings_storage_eeprom::set_eeprom_i2c_addr(_eeprom_i2c_addr);
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

void settings_storage_eeprom::set_eeprom_i2c_addr(const int _eeprom_i2c_addr)
{
    settings_storage_eeprom::eeprom_i2c_addr = _eeprom_i2c_addr;
}

uint8_t settings_storage_eeprom::read(settings_storage::SETTING_ENTRY _entry)
{
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    uint8_t buf_rx[1] = {0};
    reg_read(i2c_default, settings_storage_eeprom::eeprom_i2c_addr, EEPROM_WRITE_OFFSET+ (uint8_t)_entry, buf_rx, 1);
   
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
    //const uint8_t buf_tx[2] = {(uint8_t)_entry, _value};
    uint8_t buf_tx[1] = {_value};
    return reg_write(i2c_default, settings_storage_eeprom::eeprom_i2c_addr, EEPROM_WRITE_OFFSET+ (uint8_t)_entry, buf_tx, 1);
}


int settings_storage_eeprom::reg_write(i2c_inst *_i2c, const uint _addr, const uint8_t _reg, uint8_t* _buf, const uint8_t _bytes)
{
    int num_bytes_read = 0;
    uint8_t msg[_bytes + 1];

    // Check to make sure caller is sending 1 or more bytes
    if (_bytes < 1)
    {
        return 0;
    }

    // Append register address to front of data packet
    msg[0] = _reg;
    for (int i = 0; i < _bytes; i++)
    {
        msg[i + 1] = _buf[i];
    }

    // Write data to register(s) over I2C
    const int ret = i2c_write_blocking(_i2c, _addr, msg, (_bytes + 1), false);

     printf("write %i %i\n", msg[1], ret);
    return num_bytes_read;
}

int settings_storage_eeprom::reg_read(i2c_inst *_i2c, const uint _addr, const uint8_t _reg, uint8_t *_buf, const uint8_t _bytes)
{
    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (_bytes < 1)
    {
        return 0;
    }

    // Read data from register(s) over I2C
    i2c_write_blocking(_i2c, _addr, &_reg, 1, true);
    num_bytes_read = i2c_read_blocking(_i2c, _addr, _buf, _bytes, false);
    printf("read %i %i\n", _buf[0], num_bytes_read);
    return num_bytes_read;
}