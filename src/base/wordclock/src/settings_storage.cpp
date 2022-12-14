#include "settings_storage.h"




void settings_storage::restore_default(){
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_FACEPLATE] = WORDCLOCK_LANGUAGE;
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_DISPLAYORIENTATION] = WORDCLOCK_DISPlAY_ORIENTATION;
    settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::SET_BRIGHTNES] = WORDCLOC_BRIGHTNESS_MODE;
}

void settings_storage::init(){

    return;
    
    if(settings_storage::read(settings_storage::SETTING_ENTRY::INVALID) != FLASH_CHECK_VALUE_START || settings_storage::read(settings_storage::SETTING_ENTRY::LENGHT) != FLASH_CHECK_VALUE_END){
      
       printf("settings_storage erase data due checksum mismatch");


        uint32_t interrupts = save_and_disable_interrupts();
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_DATA_COUNT);//FLASH_SECTOR_COUNT);
        restore_interrupts(interrupts);

        //ERASE CONTENT
        for (int e = (int)settings_storage::SETTING_ENTRY::INVALID; e < (int)settings_storage::SETTING_ENTRY::LENGHT; e++) {
            settings_storage::storage_data[e] = 0;
        }

        settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::INVALID] = FLASH_CHECK_VALUE_START;
        settings_storage::storage_data[(int)settings_storage::SETTING_ENTRY::LENGHT] = FLASH_CHECK_VALUE_END;

        restore_default();
#ifdef ENABLE_FLASH_AS_EEPROM
        interrupts = save_and_disable_interrupts();
        flash_range_program(FLASH_TARGET_OFFSET, settings_storage::storage_data, FLASH_DATA_COUNT);
        restore_interrupts(interrupts);
#endif

   }
    
    
}

uint8_t settings_storage::read(settings_storage::SETTING_ENTRY _entry){
#ifdef ENABLE_FLASH_AS_EEPROM
    const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    return flash_target_contents[(int)_entry];
#else
    return settings_storage::storage_data[(int)_entry];
#endif
}

bool settings_storage::write(settings_storage::SETTING_ENTRY _entry, uint8_t _value){
    //DONT WRITE TO FIRST/LAST ELEMENT DUE WE USE IT FOR THE ERASE CHECK
    if(_entry == settings_storage::SETTING_ENTRY::INVALID || _entry == settings_storage::SETTING_ENTRY::LENGHT){
        return false;
    }

    
    //STORE IN ARRAY
    settings_storage::storage_data[(int)_entry] = _value;

    //WRITE
#ifdef ENABLE_FLASH_AS_EEPROM
    const uint32_t interrupts = save_and_disable_interrupts();
    flash_range_program(FLASH_TARGET_OFFSET, settings_storage::storage_data, FLASH_DATA_COUNT);
    restore_interrupts(interrupts);
#endif
    return true;
}