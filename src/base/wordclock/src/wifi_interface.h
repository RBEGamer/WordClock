#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__

#include <vector>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <queue>
#include "helper.h"
// ADD ARDUINO SUPPORT

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

#define DISABLE_CRC

#ifndef DISABLE_CRC
#define ENABLE_CRC
#endif

#define CMD_START_CHARACTER '%'

class wifi_interface
{
public:
    enum class CMD_INDEX
    {
        INVALID, // DONT DELETE
        LOG,
        TIME,
        BRIGHTNESS,
        DISPLAYIP,
        FACEPLATE,
        DISPLAYORIENTATION,
        BRIGHTNESSCURVE,
        DATE,
        DAYLIGHTSAVING,
        COLORMODE,
        RESTORESETTINGS,
        LENGHT // DONT DELETE
    };

    struct rxcmd
    {
        bool cmdok;
        CMD_INDEX cmd;
        std::string payload;
        rxcmd()
        {
            cmd = CMD_INDEX::INVALID;
            payload = "";
            cmdok = false;
        }
    };

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    static inline const std::string CMD_LUT[((int)CMD_INDEX::LENGHT)] = {
        "",    // INVALID
        "log", // LOG
        "st",  // TIME
        "sb",  // BRIGHTNESS
        "ip",  // DISPLAY_IP
        "sfp", // FACEPLATE
        "sdo", // DISPLAYORIENTATION
        "sbc", // BRIGHTNESS CURVE
        "sd",  // DATE
        "dls", // DAYLIGHTSAVING
        "col", // COLORMODE
        "res", // RESTORESETTINGS
    };

    // MAP FOR FAST LUT  FKT <> CMD_LUT will be used in parse_cmd
    static inline std::unordered_map<std::string, std::function<void(const std::string _payload)>> RX_CALLBACK_FUNCTIONS = {};
    static inline std::queue<std::string> rx_recieved_queue;
    static inline std::string cmd_rx_buffer = "";
    static inline std::string cmd_rx_buffer_tmp = "";
    static inline bool cmd_started = false;
    static inline bool callback_setup = false;

    // static rxcmd manual_uart_rx();
    static void process_cmd();
    static rxcmd check_extract_cmd(const std::string _cmd_rx_buffer);
    static rxcmd process_cmd_str(const std::string _rx_buffer);
    static void init_uart();
    static void on_wifi_uart_rx(); // IRQ TO COLLECT CHARS FROM RX
    static void enable_uart_irq(bool _irq_state);
    static void send_cmd_str(const CMD_INDEX _cmd, const std::string _payload);
    
#ifdef ENABLE_CRC
    static uint16_t crc16(const std::string _data, const uint16_t _poly);
#endif
    static void send_log(const std::string _payload);
    static bool register_rx_callback(const std::function<void(const std::string _payload)> _callback, const CMD_INDEX _cmd);
    // TODO IMPLEMENT COMMAND LIST

    // IMPLEMENT FOR NEW COMMANDS TO MAKE USAGE EASIER
    static void send_time(const int _h, const int _m, const int _s);
    static void send_date(const int _day, const int _month, const int _year);
    static void send_brightness(const int _b);
    static void send_brightnesscurve(const int _bc);
    static void send_daylightsaving(const bool _dls);
};
#endif