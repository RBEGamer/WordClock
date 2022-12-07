#include "pico/binary_info.h"

bi_program_build_date_string(__DATE__);
bi_1pin_with_name(PICO_DEFAULT_UART_TX_PIN, "to wifimodule rx"); 
bi_1pin_with_name(PICO_DEFAULT_UART_RX_PIN, "to wifimodule tx"); 
bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "status led out");
bi_1pin_with_name(PICO_DEFAULT_WS2812_PIN, "ws2812b data pin"); 
bi_1pin_with_name(PICO_DEFAULT_I2C_SDA_PIN, "sda bh1750, ds1307 m24c02"); 
bi_1pin_with_name(PICO_DEFAULT_I2C_SCL_PIN, "scl bh1750, ds1307 m24c02"); 