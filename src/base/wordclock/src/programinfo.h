#include "pico/binary_info.h"

#define bi_program_build_date_string(__DATE__)



#define bi_1pin_with_name(PICO_DEFAULT_UART_TX_PIN, "to wifimodule rx") 
#define bi_1pin_with_name(PICO_DEFAULT_UART_RX_PIN, "to wifimodule tx") 

#define bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "status led out") 
#define bi_1pin_with_name(PICO_DEFAULT_WS2812_PIN, "ws2812b data pin") 

#define bi_1pin_with_name(PICO_DEFAULT_I2C_SDA_PIN, "sda bh1750, ds1307") 
#define bi_1pin_with_name(PICO_DEFAULT_I2C_SCL_PIN, "scl bh1750, ds1307") 
