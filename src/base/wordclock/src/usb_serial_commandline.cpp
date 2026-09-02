#include "usb_serial_commandline.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>

namespace
{
std::string trim(const std::string &_value)
{
    const auto first = std::find_if_not(_value.begin(), _value.end(), [](const unsigned char ch) {
        return std::isspace(ch);
    });
    const auto last = std::find_if_not(_value.rbegin(), _value.rend(), [](const unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (first >= last)
    {
        return "";
    }
    return std::string(first, last);
}

std::string lowercase(std::string _value)
{
    std::transform(_value.begin(), _value.end(), _value.begin(), [](const unsigned char ch) {
        return (char)std::tolower(ch);
    });
    return _value;
}

bool parse_time(const std::string &_value, int &_hour, int &_minute, int &_second)
{
    size_t position = 0;
    const auto parse_component = [&_value, &position](int &_component, const int _maximum) {
        _component = 0;
        size_t digit_count = 0;
        while (position < _value.length() &&
               std::isdigit((unsigned char)_value[position]) &&
               digit_count < 2)
        {
            _component = (_component * 10) + (_value[position] - '0');
            position++;
            digit_count++;
        }
        return digit_count > 0 &&
               (position == _value.length() || !std::isdigit((unsigned char)_value[position])) &&
               _component <= _maximum;
    };

    if (!parse_component(_hour, 23) || position >= _value.length() || _value[position++] != ':' ||
        !parse_component(_minute, 59) || position >= _value.length() || _value[position++] != ':' ||
        !parse_component(_second, 59) || position != _value.length())
    {
        return false;
    }
    return true;
}
} // namespace

void usb_serial_commandline::init(const set_time_callback _set_time,
                                  const set_ignore_wifi_time_callback _set_ignore_wifi_time,
                                  const get_ignore_wifi_time_callback _get_ignore_wifi_time)
{
    set_time = _set_time;
    set_ignore_wifi_time = _set_ignore_wifi_time;
    get_ignore_wifi_time = _get_ignore_wifi_time;
    command_buffer.clear();
    printf("USB command line ready; enter 'help' for commands.\n");
}

void usb_serial_commandline::process()
{
    // Limit work per main-loop iteration so a busy USB host cannot starve the clock display.
    for (size_t i = 0; i < MAX_COMMAND_LENGTH; ++i)
    {
        const int input = getchar_timeout_us(0);
        if (input == PICO_ERROR_TIMEOUT)
        {
            return;
        }

        const char ch = (char)input;
        if (ch == '\r' || ch == '\n')
        {
            if (!command_buffer.empty())
            {
                process_line(command_buffer);
                command_buffer.clear();
            }
        }
        else if (ch == '\b' || ch == 0x7f)
        {
            if (!command_buffer.empty())
            {
                command_buffer.pop_back();
            }
        }
        else if (std::isprint((unsigned char)ch))
        {
            if (command_buffer.length() < MAX_COMMAND_LENGTH)
            {
                command_buffer += ch;
            }
            else
            {
                command_buffer.clear();
                printf("ERROR command is too long\n");
            }
        }
    }
}

void usb_serial_commandline::process_line(const std::string &_line)
{
    const std::string line = trim(_line);
    const size_t separator = line.find_first_of(" \t");
    const std::string command = lowercase(line.substr(0, separator));
    const std::string argument = separator == std::string::npos ? "" : trim(line.substr(separator + 1));

    if (command == "help")
    {
        print_help();
    }
    else if (command == "time")
    {
        int hour = 0;
        int minute = 0;
        int second = 0;
        if (!parse_time(argument, hour, minute, second))
        {
            printf("ERROR usage: time HH:MM:SS (00:00:00 to 23:59:59)\n");
            return;
        }
        if (!set_time)
        {
            printf("ERROR time setter is unavailable\n");
            return;
        }
        set_time(hour, minute, second);
        printf("OK time set to %02d:%02d:%02d\n", hour, minute, second);
    }
    else if (command == "wifi-time-ignore")
    {
        if (argument.empty())
        {
            if (!get_ignore_wifi_time)
            {
                printf("ERROR Wi-Fi time setting is unavailable\n");
                return;
            }
            printf("wifi-time-ignore: %s\n", get_ignore_wifi_time() ? "on" : "off");
            return;
        }

        const std::string value = lowercase(argument);
        if (value != "on" && value != "off")
        {
            printf("ERROR usage: wifi-time-ignore on|off\n");
            return;
        }
        if (!set_ignore_wifi_time)
        {
            printf("ERROR Wi-Fi time setting is unavailable\n");
            return;
        }
        set_ignore_wifi_time(value == "on");
        printf("OK wifi-time-ignore: %s\n", value.c_str());
    }
    else if (!command.empty())
    {
        printf("ERROR unknown command '%s'; enter 'help'\n", command.c_str());
    }
}

void usb_serial_commandline::print_help()
{
    printf("Commands:\n");
    printf("  time HH:MM:SS          Set the clock time\n");
    printf("  wifi-time-ignore on    Ignore time updates from the Wi-Fi module\n");
    printf("  wifi-time-ignore off   Accept time updates from the Wi-Fi module\n");
    printf("  wifi-time-ignore       Show the current setting\n");
    printf("  help                   Show this help\n");
}
