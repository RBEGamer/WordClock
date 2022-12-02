#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__


#include <vector>
#include <cstring>
#include <functional>
#include <unordered_map>

#include "helper.h"
// ADD ARDUINO SUPPORT
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(ESP32)
#define IS_ARDUINO
#define std::string String
#define printf() Serial.printf(x)
#define PICO_DEFAULT_UART_BAUDRATE 9600
#else
#define IS_PICO
#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#if defined(PICO_DEFAULT_UART0)
#define UART_WIFI uart0
#define UART_WIFI_IRQ UART0_IRQ
#elif defined(PICO_DEFAULT_UART1)
#define UART_WIFI uart1
#define UART_WIFI_IRQ UART1_IRQ
#endif


#endif









//#define DISABLE_CRC false
#define DISABLE_CRC true
#define CMD_START_CHARACTER '%'




class wifi_interface
{
public:
    struct rxcmd
    {
        bool cmdok;
        std::string cmd;
        std::string payload;
    };


    enum class CMD_INDEX
    {
        CURRENT_TIME,
        CURRENT_BRIGHTNES,
        LOG,
        SET_TIME,
        SET_BRIGHTNES,
        DISPLAY_IP,
        SET_FACEPLATE,
        SET_DISPLAYORIENTATION,
        LENGHT                // DONT DELETE
    };                    

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    static inline const std::string CMD_LUT[((int)CMD_INDEX::LENGHT)] = {
        "ct",       // CURRENT_TIME
        "cb",       // CURRENT_BRIGHTNES
        "log",      // LOG
        "st",       // SET_TIME
        "sb",       // SET_BRIGHTNES
        "ip",       // DISPLAY_IP
        "sfp",      // SET_FACEPLATE
        "sdo"       // SET_DISPLAYORIENTATION
    };


    // MAP FOR FAST LUT  FKT <> CMD_LUT will be used in parse_cmd
    static inline std::unordered_map<std::string, std::function<void(const std::string _payload)>> RX_CMD_CALLBACK_LUT = {};



   
    
    static inline std::string cmd_rx_buffer = "";
    static inline bool cmd_rx_complete = false;
    static inline bool cmd_cmd_started = false;
    static inline bool prev_irq_state = false;
    static inline bool callback_setup = false;
    static inline bool cmd_rx_started = false;


    static rxcmd manual_uart_rx();
    static rxcmd parse_cmd(const std::string _cmd_rx_buffer, const bool _cmd_rx_complete);
    static rxcmd parse_cmd();
    static void init_uart();
    static void on_wifi_uart_rx(); // IRQ TO COLLECT CHARS FROM RX
    static void enable_uart_irq(bool _irq_state);
    static void send_cmd_str(const CMD_INDEX _cmd _cmd, const std::string _payload);
    static uint16_t crc16(const std::string _data, const uint16_t _poly);
    static void send_log(const std::string _payload);
     static bool register_rx_callback(const std::function<void(const std::string _payload)> _callback, const CMD_INDEX _cmd);
    // TODO IMPLEMENT COMMAND LIST


    // IMPLEMENT FOR NEW COMMANDS TO MAKE USAGE EASIER
    static void send_current_time(const int _h, const int _m, const int _s);
    static void send_temperature(const int _t);
    static void send_brightness(const int _b);
    

    
};
#endif