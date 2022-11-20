#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__

#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#if defined(PICO_DEFAULT_UART0)
#define UART_WIFI uart0
#elif defined(PICO_DEFAULT_UART1)
#define UART_WIFI uart1
#endif


class wifi_interface
{
public:
static void init_uart();
static void send_current_time(const int _h, const int _m, const int _s);
static void send_temperature(const int _t);
static void send_brightness(const int _b);
static uint16_t crc16(const std::string _data, const uint16_t _poly);
static void send_cmd_str(const std::string _command, const std::string _payload);
static void send_log(const std::string _payload);
};
#endif