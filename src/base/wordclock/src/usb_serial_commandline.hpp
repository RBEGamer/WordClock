#ifndef __USB_SERIAL_COMMANDLINE_HPP__
#define __USB_SERIAL_COMMANDLINE_HPP__

#include <functional>
#include <string>

#include "pico/stdlib.h"

class usb_serial_commandline
{
public:
    using set_time_callback = std::function<void(const int, const int, const int)>;
    using set_ignore_wifi_time_callback = std::function<void(const bool)>;
    using get_ignore_wifi_time_callback = std::function<bool()>;

    static void init(const set_time_callback _set_time,
                     const set_ignore_wifi_time_callback _set_ignore_wifi_time,
                     const get_ignore_wifi_time_callback _get_ignore_wifi_time);
    static void process();

private:
    static constexpr size_t MAX_COMMAND_LENGTH = 80;

    static inline std::string command_buffer = "";
    static inline set_time_callback set_time = nullptr;
    static inline set_ignore_wifi_time_callback set_ignore_wifi_time = nullptr;
    static inline get_ignore_wifi_time_callback get_ignore_wifi_time = nullptr;

    static void process_line(const std::string &_line);
    static void print_help();
};

#endif
