#include "settings_storage_flash.hpp"

settings_storage_flash::settings_storage_flash()
{
}

settings_storage_flash::~settings_storage_flash()
{
}

void settings_storage_flash::init()
{
}

void settings_storage_flash::format()
{
    const uint32_t interrupts = save_and_disable_interrupts();
    for (size_t i = 0; i < REQUIRED_SECTORS; i++)
    {
        flash_range_erase(FLASH_TARGET_OFFSET + (i * FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);
    }
    restore_interrupts(interrupts);
}

uint8_t settings_storage_flash::read(settings_storage::SETTING_ENTRY _entry)
{
    if (_entry < settings_storage::SETTING_ENTRY::INVALID || _entry > settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }

    return settings_storage_flash::read_unsafe((int)_entry);
}

uint8_t settings_storage_flash::read_unsafe(const unsigned int _entry)
{
    const int addr = XIP_BASE + FLASH_TARGET_OFFSET;
    const uint8_t *p = (uint8_t *)addr;
    const uint8_t value = p[_entry];
    return value;
}

bool settings_storage_flash::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE OUTSIDE RANGE
    if (_entry < settings_storage::SETTING_ENTRY::INVALID || _entry > settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }

    uint8_t buf[FLASH_PAGE_SIZE * REQUIRED_PAGES] = { 0 };
    for (size_t i = (size_t)settings_storage::SETTING_ENTRY::INVALID; i < (size_t)settings_storage::SETTING_ENTRY::LENGHT+1; i++)
    {
            buf[i] = read_unsafe(i);
    }
    buf[(int)_entry] = _value;

    format();

    const uint32_t interrupts = save_and_disable_interrupts();
    for (size_t i = 0; i < REQUIRED_PAGES; i++)
    {
        flash_range_program(FLASH_TARGET_OFFSET + (i * FLASH_PAGE_SIZE), (uint8_t *)(buf + (i * FLASH_PAGE_SIZE)), FLASH_PAGE_SIZE);
    }
    restore_interrupts(interrupts);

    return true;
}