#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__

#include <string>
#include <vector>
#include <cstring>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "helper.h"
#if defined(PICO_DEFAULT_UART0)
#define UART_WIFI uart0
#define UART_WIFI_IRQ UART0_IRQ
#elif defined(PICO_DEFAULT_UART1)
#define UART_WIFI uart1
#define UART_WIFI_IRQ UART1_IRQ
#endif

class wifi_interface
{
public:
    struct rxcmd
    {
        bool cmdok;
        std::string cmd;
        std::string payload;
    };

    //see wifi_interface.cpp
    static std::string cmd_rx_buffer;
    static bool cmd_rx_complete;
    static rxcmd manual_uart_rx();
    static rxcmd parse_cmd(const std::string _cmd_rx_buffer, const bool _cmd_rx_complete);
    static rxcmd parse_cmd();
    static void init_uart();
    static void on_wifi_uart_rx(); // IRQ TO COLLECT CHARS FROM RX
    static void enable_uart_irq(bool _irq_state);
    static void send_current_time(const int _h, const int _m, const int _s);
    static void send_temperature(const int _t);
    static void send_brightness(const int _b);
    static uint16_t crc16(const std::string _data, const uint16_t _poly);
    static void send_cmd_str(const std::string _command, const std::string _payload);
    static void send_log(const std::string _payload);

    
};
#endif