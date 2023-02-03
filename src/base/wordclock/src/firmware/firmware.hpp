#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

// https://docs.espressif.com/projects/esptool/en/latest/esp32s3/advanced-topics/serial-protocol.html#writing-data
#include "pico/stdlib.h"

//x
// #define INCLUDE_ESP_FIRMWARE
#include "firmware_data.hpp"

#ifndef ESP_RST_GPIO
#define ESP_RST_GPIO 5
#endif

#ifndef ESP_BOOT_GPIO
#define ESP_BOOT_GPIO 6
#endif

#if defined(PICO_DEFAULT_UART0)
#define UART_FWUPDATE uart0
#elif defined(PICO_DEFAULT_UART1)
#define UART_FWUPDATE uart1
#endif

//DEFAULT ESP BOOTMODE BAUDRATE
#define UART_FWUPDATE_BAUDRATE 115200

// https://gist.github.com/shadowandy/af468d38ed7ab3ff718b

class firmware{

    static const void prepare_gpio(); // SET GPIO DIRECTIONS
    static const void reset_gpio(); // RESET GPIO TO INPUT
    static const void trigger_flashmode();
    static const bool trigger_reset();
    public:
        static const bool flash();
        static const bool reset(); //RESET / RESTART MODULE
        
};
//RECONFIG UART
// PERORM RESET
#endif