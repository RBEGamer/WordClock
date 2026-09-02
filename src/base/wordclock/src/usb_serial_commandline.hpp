#ifndef __USB_SERIAL_COMMANDLINE_HPP__
#define __USB_SERIAL_COMMANDLINE_HPP__

#include <functional>
#include <string>

#include "pico/stdlib.h"

class usb_serial_commandline
{
public:
    using set_time_callback = std::function<void(const int, const int, const int)>;
    using set_string_callback = std::function<void(const std::string &)>;
    using set_bool_callback = std::function<void(const bool)>;
    using get_bool_callback = std::function<bool()>;
    using set_integer_callback = std::function<void(const int)>;
    using get_integer_callback = std::function<int()>;

    struct callbacks
    {
        set_time_callback set_time = nullptr;
        set_string_callback set_date = nullptr;
        set_string_callback set_brightness = nullptr;
        set_string_callback display_ip = nullptr;
        set_string_callback set_faceplate = nullptr;
        set_string_callback set_orientation = nullptr;
        set_string_callback set_brightness_curve = nullptr;
        set_string_callback set_daylight_saving = nullptr;
        set_string_callback set_color_mode = nullptr;
        set_string_callback restore_settings = nullptr;
        set_string_callback set_dot_brightness = nullptr;
        set_string_callback set_blink_dots = nullptr;
        set_bool_callback set_ignore_wifi_time = nullptr;
        get_bool_callback get_ignore_wifi_time = nullptr;
        set_integer_callback set_timezone_offset = nullptr;
        get_integer_callback get_timezone_offset = nullptr;
    };

    static void init(const callbacks &_callbacks);
    static void process();

private:
    static constexpr size_t MAX_COMMAND_LENGTH = 80;

    static inline std::string command_buffer = "";
    static callbacks command_callbacks;

    static void process_line(const std::string &_line);
    static void print_help();
};

#endif
