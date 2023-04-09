#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__

#include <vector>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <queue>

#include "helper.h"
#include "wordclock_faceplate/wordclock_faceplate.hpp"
// ADD ARDUINO SUPPORT

#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#if defined(PICO_DEFAULT_UART0)
#define UART_WIFI uart0
#elif defined(PICO_DEFAULT_UART1)
#define UART_WIFI uart1
#endif

#define DISABLE_CRC

#ifndef DISABLE_CRC
#define ENABLE_CRC
#endif

#define CMD_START_CHARACTER '%'
#define CMD_DEFINITION_TOUPLE_SEPERATION_CHARACTER '#'
#define CMD_DEFINITION_OPTION_SEPERATION_CHARACTER ','
class wifi_interface
{
public:
    enum class CMD_DATATYPE
    {
        INVISIBLE, // USER CANT EDIT THIS ENTRY ITS USED FOR STATUS MESSAGES
        NUMBER,
        BOOL,
        STRING,
        OPTIONS,
        CMD_DEFINITION,
        LENGHT // DONT DELETE
    };

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
        DOTBRIGHTNESS,
        BLINKENDOTS,

        // DONT DELETE FOLLOWING
        SYS_SEND_COMMANDS,
        SYS_CMD_DEFINITION,
        LENGHT // DONT DELETE
    };

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    // touple ID, USER FIREDNLY NAME, DATATYPE, min/lenght, max, user visible, options
    static const int CMD_LUT_SIZE = ((int)CMD_INDEX::LENGHT);
    static inline const std::tuple<const std::string, const std::string, CMD_DATATYPE, int, int, bool, std::vector<std::string>> CMD_LUT[CMD_LUT_SIZE] = {
        {"", "", CMD_DATATYPE::INVISIBLE, -1, -1, false, {}},                        // INVALID
        {"log", "LOG", CMD_DATATYPE::STRING, -1, -1, false, {}},                            // LOG
        {"st", "SET TIME", CMD_DATATYPE::STRING, -1, -1, false, {}},                        // SET TIME
        {"sb", "SET BRIGHTNESS", CMD_DATATYPE::NUMBER, 0, -255, true, {}},                  // SET BRIGHTNESS
        {"ip", "", CMD_DATATYPE::STRING, -1, -1, false, {}},                      // DISPLAY_IP
        {"sfp", "SET FACEPLATE", CMD_DATATYPE::OPTIONS, 0, 5, true, {"GERMAN", "ENGLISH", "ITALIAN", "BELGIAN", "BINARY", "DOTS"}}, // SET FACEPLATE
        {"sdo", "ROTATE DISPLAY", CMD_DATATYPE::BOOL, -1, -1, true, {}},                    // DISPLAYORIENTATION
        {"sbc", "BRIGHTNESS CURVE", CMD_DATATYPE::NUMBER, 10, 100, true, {}},               // BRIGHTNESS CURVE
        {"sd", "", CMD_DATATYPE::STRING, -1, -1, false, {}},                                // DATE
        {"dls", "DAYLIGHTSAVING", CMD_DATATYPE::BOOL, -1, -1, true, {}},                    // DAYLIGHTSAVING
        {"col", "COLORMODE", CMD_DATATYPE::OPTIONS, 0, 3, true, {"RAINBOW SEPERATE", "RAINBOW EQUAL", "WARM WHITE", "COLD WHITE"}},                       // COLORMODE
        {"res", "RESTORE CLOCK SETTINGS", CMD_DATATYPE::BOOL, -1, -1, true, {}},            // RESTORESETTINGS
        {"dob", "DOT BRIGHTNESS", CMD_DATATYPE::NUMBER, 0, 100, true, {}},      // DOTBRIGHTNESS
        {"bld", "BLINKENDOTS", CMD_DATATYPE::BOOL, -1, -1, true, {}},                       // BLINKENDOTS

        // DONT DELETE FOLLOWING
        {"syssc", "", CMD_DATATYPE::INVISIBLE, -1, -1, false, {}},           // SYS_SEND_COMMANDS USES TO SEND BACK COMMAND LIST
        {"syscmddef", "", CMD_DATATYPE::CMD_DEFINITION, -1, -1, false, {}}, // SYS_SEND_COMMANDS USES TO SEND BACK COMMAND LIST
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

    static void send_present_command_list(const std::string _payload); // sends implemented commands to wifimodule to generate user ui

    // MAP FOR FAST LUT  FKT <> CMD_LUT will be used in parse_cmd
    static inline std::unordered_map<std::string, std::function<void(const std::string _payload)>> RX_CALLBACK_FUNCTIONS = {
        {"syssc", wifi_interface::send_present_command_list}, // ALWAYS NEEDED TO TRIGGER CMD OPTIONS LIST TO WIFIINTERFACE
    };

    static inline std::queue<std::string> rx_recieved_queue;
    static inline std::string cmd_rx_buffer = "";
    static inline std::string cmd_rx_buffer_tmp = "";
    static inline bool cmd_started = false;
    static inline bool callback_setup = false;

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