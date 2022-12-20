#ifndef __SETTINGS_STORAGE_FLASH_H__
#define __SETTINGS_STORAGE_FLASH_H__

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/flash.h" // for the flash erasing and writing
#include "hardware/sync.h"  // for the interrupts

#include "settings_storage.hpp"

extern char __flash_binary_end;
class settings_storage_flash : public settings_storage
{

public:
    const int SETTINGS_COUNT = ((int)SETTING_ENTRY::LENGHT) + 1;                           //+1 we want to write to LENGHT too
    const int REQUIRED_PAGES = ((sizeof(uint8_t) / FLASH_PAGE_SIZE) / SETTINGS_COUNT) + 1; // ALIGN TO 256byte PAGES
    const uint32_t FLASH_DATA_COUNT = FLASH_PAGE_SIZE * REQUIRED_PAGES;                    // FINAL REQUIRED SPACE IN 256byte * NEEDED PAGES
    const uint32_t REQUIRED_SECTORS = (FLASH_DATA_COUNT / FLASH_SECTOR_SIZE) + 1;
    const uint32_t FLASH_SECTOR_BYTES = FLASH_SECTOR_SIZE * REQUIRED_SECTORS;
    const size_t SIZE = ((FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1)); // 256 byte boundries
    const size_t FLASH_TARGET_OFFSET = (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_BYTES);

protected:
    uint8_t read_unsafe(const unsigned int _entry);

public:
    settings_storage_flash();
    ~settings_storage_flash();

    void init() override;
    void format() override;
    uint8_t read(SETTING_ENTRY _entry) override;
    bool write(SETTING_ENTRY _entry, uint8_t _value) override;
};

#endif
