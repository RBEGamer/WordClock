#include "settings_storage_flash.hpp"

settings_storage_flash::settings_storage_flash()
{
}

settings_storage_flash::~settings_storage_flash()
{
}

void settings_storage_flash::format()
{
    const uint32_t interrupts = save_and_disable_interrupts();
    flash_range_erase((intptr_t)FLASH_TARGET_OFFSET - (intptr_t)XIP_BASE, ERASE_SIZE);
    restore_interrupts(interrupts);
}

uint8_t settings_storage_flash::read(settings_storage::SETTING_ENTRY _entry)
{
    const uint8_t *flash_target_contents = (const uint8_t *)((intptr_t)FLASH_TARGET_OFFSET - (intptr_t)XIP_BASE);
    return flash_target_contents[(int)_entry];
}

bool settings_storage_flash::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value)
{
    // DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if (_entry < settings_storage::SETTING_ENTRY::INVALID || _entry > settings_storage::SETTING_ENTRY::LENGHT)
    {
        return false;
    }
    // STORE
    uint8_t storage_data_flash[SIZE] = { 0 };
    // WRITE
    const uint32_t interrupts = save_and_disable_interrupts();
    flash_range_program((intptr_t)FLASH_TARGET_OFFSET - (intptr_t)XIP_BASE, storage_data_flash, SIZE);
    restore_interrupts(interrupts);

    return true;
}