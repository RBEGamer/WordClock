#include "usb_serial_commandline.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstdlib>

usb_serial_commandline::callbacks usb_serial_commandline::command_callbacks;

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

std::string normalize_option(std::string _value)
{
    _value = lowercase(trim(_value));
    std::replace(_value.begin(), _value.end(), '_', '-');
    std::replace(_value.begin(), _value.end(), ' ', '-');
    return _value;
}

bool parse_unsigned_integer(const std::string &_value, const int _minimum, const int _maximum, int &_result)
{
    if (_value.empty())
    {
        return false;
    }

    int result = 0;
    for (const char ch : _value)
    {
        if (!std::isdigit((unsigned char)ch))
        {
            return false;
        }
        const int digit = ch - '0';
        if (digit > _maximum || result > (_maximum - digit) / 10)
        {
            return false;
        }
        result = (result * 10) + digit;
    }

    if (result < _minimum)
    {
        return false;
    }
    _result = result;
    return true;
}

bool parse_time(const std::string &_value, int &_hour, int &_minute, int &_second)
{
    const size_t first_separator = _value.find(':');
    const size_t second_separator = first_separator == std::string::npos
                                        ? std::string::npos
                                        : _value.find(':', first_separator + 1);
    if (first_separator == std::string::npos ||
        second_separator == std::string::npos ||
        _value.find(':', second_separator + 1) != std::string::npos)
    {
        return false;
    }

    return parse_unsigned_integer(_value.substr(0, first_separator), 0, 23, _hour) &&
           parse_unsigned_integer(_value.substr(first_separator + 1, second_separator - first_separator - 1), 0, 59, _minute) &&
           parse_unsigned_integer(_value.substr(second_separator + 1), 0, 59, _second);
}

bool leap_year(const int _year)
{
    return _year > 0 && _year % 4 == 0 && (_year % 100 != 0 || _year % 400 == 0);
}

int days_in_month(const int _month, const int _year)
{
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return _month == 2 && leap_year(_year) ? 29 : days[_month - 1];
}

bool parse_date(const std::string &_value, int &_day, int &_month, int &_year)
{
    const size_t first_separator = _value.find('.');
    const size_t second_separator = first_separator == std::string::npos
                                        ? std::string::npos
                                        : _value.find('.', first_separator + 1);
    if (first_separator == std::string::npos ||
        second_separator == std::string::npos ||
        _value.find('.', second_separator + 1) != std::string::npos ||
        !parse_unsigned_integer(_value.substr(0, first_separator), 1, 31, _day) ||
        !parse_unsigned_integer(_value.substr(first_separator + 1, second_separator - first_separator - 1), 1, 12, _month) ||
        !parse_unsigned_integer(_value.substr(second_separator + 1), 2000, 2099, _year))
    {
        return false;
    }
    return _day <= days_in_month(_month, _year);
}

bool parse_bool(const std::string &_value, bool &_result)
{
    const std::string value = lowercase(_value);
    if (value == "on" || value == "enabled" || value == "true" || value == "1")
    {
        _result = true;
        return true;
    }
    if (value == "off" || value == "disabled" || value == "false" || value == "0")
    {
        _result = false;
        return true;
    }
    return false;
}

template <size_t N>
bool parse_option(const std::string &_value, const std::array<const char *, N> &_names, int &_result)
{
    if (parse_unsigned_integer(_value, 0, (int)N - 1, _result))
    {
        return true;
    }

    const std::string value = normalize_option(_value);
    for (size_t i = 0; i < N; ++i)
    {
        if (value == _names[i])
        {
            _result = (int)i;
            return true;
        }
    }
    return false;
}

bool parse_ipv4(const std::string &_value)
{
    size_t start = 0;
    for (int component = 0; component < 4; ++component)
    {
        const size_t separator = _value.find('.', start);
        if ((component < 3 && separator == std::string::npos) ||
            (component == 3 && separator != std::string::npos))
        {
            return false;
        }

        const size_t end = separator == std::string::npos ? _value.length() : separator;
        int parsed_component = 0;
        if (!parse_unsigned_integer(_value.substr(start, end - start), 0, 255, parsed_component))
        {
            return false;
        }
        start = end + 1;
    }
    return true;
}

bool parse_timezone(const std::string &_value, int &_offset_minutes)
{
    const std::string value = lowercase(trim(_value));
    if (value == "utc" || value == "z" || value == "0")
    {
        _offset_minutes = 0;
        return true;
    }
    if (value.length() < 2 || (value[0] != '+' && value[0] != '-'))
    {
        return false;
    }

    const size_t separator = value.find(':', 1);
    int hours = 0;
    int minutes = 0;
    if (separator == std::string::npos)
    {
        if (!parse_unsigned_integer(value.substr(1), 0, 14, hours))
        {
            return false;
        }
    }
    else if (value.find(':', separator + 1) != std::string::npos ||
             !parse_unsigned_integer(value.substr(1, separator - 1), 0, 14, hours) ||
             !parse_unsigned_integer(value.substr(separator + 1), 0, 59, minutes))
    {
        return false;
    }

    if (minutes % 15 != 0)
    {
        return false;
    }
    int total_minutes = (hours * 60) + minutes;
    if (value[0] == '-')
    {
        total_minutes = -total_minutes;
    }
    if (total_minutes < -12 * 60 || total_minutes > 14 * 60)
    {
        return false;
    }

    _offset_minutes = total_minutes;
    return true;
}

void print_timezone(const int _offset_minutes)
{
    const char sign = _offset_minutes < 0 ? '-' : '+';
    const int absolute_minutes = std::abs(_offset_minutes);
    printf("%c%02d:%02d", sign, absolute_minutes / 60, absolute_minutes % 60);
}
} // namespace

void usb_serial_commandline::init(const callbacks &_callbacks)
{
    command_callbacks = _callbacks;
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
    static const std::array<const char *, 7> faceplates = {
        "german", "english", "italian", "belgian", "binary", "dots", "test"};
    static const std::array<const char *, 4> color_modes = {
        "rainbow-separate", "rainbow-same", "cold-white", "warm-white"};

    const std::string line = trim(_line);
    const size_t separator = line.find_first_of(" \t");
    const std::string command = lowercase(line.substr(0, separator));
    const std::string argument = separator == std::string::npos ? "" : trim(line.substr(separator + 1));

    if (command == "help")
    {
        print_help();
    }
    else if (command == "time" || command == "st")
    {
        int hour = 0;
        int minute = 0;
        int second = 0;
        if (!parse_time(argument, hour, minute, second))
        {
            printf("ERROR usage: time HH:MM:SS (00:00:00 to 23:59:59)\n");
        }
        else if (!command_callbacks.set_time)
        {
            printf("ERROR time setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_time(hour, minute, second);
            printf("OK time: %02d:%02d:%02d\n", hour, minute, second);
        }
    }
    else if (command == "date" || command == "sd")
    {
        int day = 0;
        int month = 0;
        int year = 0;
        if (!parse_date(argument, day, month, year))
        {
            printf("ERROR usage: date DD.MM.YYYY (2000 to 2099)\n");
        }
        else if (!command_callbacks.set_date)
        {
            printf("ERROR date setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_date(argument);
            printf("OK date: %02d.%02d.%04d\n", day, month, year);
        }
    }
    else if (command == "brightness" || command == "sb")
    {
        int brightness = 0;
        const bool automatic = lowercase(argument) == "auto";
        if (!automatic &&
            (!parse_unsigned_integer(argument, 0, 255, brightness) || (brightness > 0 && brightness < 10)))
        {
            printf("ERROR usage: brightness auto|0|10..255\n");
        }
        else if (!command_callbacks.set_brightness)
        {
            printf("ERROR brightness setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_brightness(automatic ? "0" : std::to_string(brightness));
            printf("OK brightness: %s\n", automatic || brightness == 0 ? "auto" : argument.c_str());
        }
    }
    else if (command == "display-ip" || command == "ip")
    {
        if (!parse_ipv4(argument))
        {
            printf("ERROR usage: display-ip A.B.C.D\n");
        }
        else if (!command_callbacks.display_ip)
        {
            printf("ERROR IP display is unavailable\n");
        }
        else
        {
            command_callbacks.display_ip(argument);
            printf("OK display-ip: %s\n", argument.c_str());
        }
    }
    else if (command == "faceplate" || command == "sfp")
    {
        int faceplate = 0;
        if (!parse_option(argument, faceplates, faceplate))
        {
            printf("ERROR usage: faceplate german|english|italian|belgian|binary|dots|test|0..6\n");
        }
        else if (!command_callbacks.set_faceplate)
        {
            printf("ERROR faceplate setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_faceplate(std::to_string(faceplate));
            printf("OK faceplate: %s\n", faceplates[faceplate]);
        }
    }
    else if (command == "orientation" || command == "sdo")
    {
        bool rotated = false;
        const std::string value = lowercase(argument);
        if (value == "normal")
        {
            rotated = false;
        }
        else if (value == "rotated")
        {
            rotated = true;
        }
        else if (!parse_bool(argument, rotated))
        {
            printf("ERROR usage: orientation normal|rotated|0|1\n");
            return;
        }

        if (!command_callbacks.set_orientation)
        {
            printf("ERROR orientation setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_orientation(rotated ? "1" : "0");
            printf("OK orientation: %s\n", rotated ? "rotated" : "normal");
        }
    }
    else if (command == "brightness-curve" || command == "sbc")
    {
        int curve = 0;
        if (!parse_unsigned_integer(argument, 10, 100, curve))
        {
            printf("ERROR usage: brightness-curve 10..100\n");
        }
        else if (!command_callbacks.set_brightness_curve)
        {
            printf("ERROR brightness curve setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_brightness_curve(std::to_string(curve));
            printf("OK brightness-curve: %d\n", curve);
        }
    }
    else if (command == "daylight-saving" || command == "dls")
    {
        bool enabled = false;
        if (!parse_bool(argument, enabled))
        {
            printf("ERROR usage: daylight-saving on|off\n");
        }
        else if (!command_callbacks.set_daylight_saving)
        {
            printf("ERROR daylight-saving setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_daylight_saving(enabled ? "1" : "0");
            printf("OK daylight-saving: %s\n", enabled ? "on" : "off");
        }
    }
    else if (command == "color-mode" || command == "col")
    {
        int color_mode = 0;
        if (!parse_option(argument, color_modes, color_mode))
        {
            printf("ERROR usage: color-mode rainbow-separate|rainbow-same|cold-white|warm-white|0..3\n");
        }
        else if (!command_callbacks.set_color_mode)
        {
            printf("ERROR color mode setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_color_mode(std::to_string(color_mode));
            printf("OK color-mode: %s\n", color_modes[color_mode]);
        }
    }
    else if (command == "dot-brightness" || command == "dob")
    {
        int brightness = 0;
        if (!parse_unsigned_integer(argument, 0, 100, brightness))
        {
            printf("ERROR usage: dot-brightness 0..100\n");
        }
        else if (!command_callbacks.set_dot_brightness)
        {
            printf("ERROR dot brightness setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_dot_brightness(std::to_string(brightness));
            printf("OK dot-brightness: %d\n", brightness);
        }
    }
    else if (command == "blink-dots" || command == "bld")
    {
        bool enabled = false;
        if (!parse_bool(argument, enabled))
        {
            printf("ERROR usage: blink-dots on|off\n");
        }
        else if (!command_callbacks.set_blink_dots)
        {
            printf("ERROR blinking-dot setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_blink_dots(enabled ? "1" : "0");
            printf("OK blink-dots: %s\n", enabled ? "on" : "off");
        }
    }
    else if (command == "wifi-time-ignore" || command == "iwt")
    {
        if (argument.empty())
        {
            if (!command_callbacks.get_ignore_wifi_time)
            {
                printf("ERROR Wi-Fi time setting is unavailable\n");
            }
            else
            {
                printf("wifi-time-ignore: %s\n", command_callbacks.get_ignore_wifi_time() ? "on" : "off");
            }
            return;
        }

        bool ignore = false;
        if (!parse_bool(argument, ignore))
        {
            printf("ERROR usage: wifi-time-ignore on|off\n");
        }
        else if (!command_callbacks.set_ignore_wifi_time)
        {
            printf("ERROR Wi-Fi time setting is unavailable\n");
        }
        else
        {
            command_callbacks.set_ignore_wifi_time(ignore);
            printf("OK wifi-time-ignore: %s\n", ignore ? "on" : "off");
        }
    }
    else if (command == "timezone" || command == "tz")
    {
        if (argument.empty())
        {
            if (!command_callbacks.get_timezone_offset)
            {
                printf("ERROR timezone setting is unavailable\n");
            }
            else
            {
                printf("timezone: ");
                print_timezone(command_callbacks.get_timezone_offset());
                printf("\n");
            }
            return;
        }

        int offset_minutes = 0;
        if (!parse_timezone(argument, offset_minutes))
        {
            printf("ERROR usage: timezone UTC|[-12:00..+14:00] in 15-minute steps\n");
        }
        else if (!command_callbacks.set_timezone_offset)
        {
            printf("ERROR timezone setter is unavailable\n");
        }
        else
        {
            command_callbacks.set_timezone_offset(offset_minutes);
            printf("OK timezone: ");
            print_timezone(offset_minutes);
            printf("\n");
        }
    }
    else if (command == "restore-settings" || command == "res")
    {
        if (lowercase(argument) != "confirm" && argument != "1")
        {
            printf("ERROR usage: restore-settings confirm\n");
        }
        else if (!command_callbacks.restore_settings)
        {
            printf("ERROR settings restore is unavailable\n");
        }
        else
        {
            command_callbacks.restore_settings("1");
            printf("OK settings restored to defaults\n");
        }
    }
    else if (!command.empty())
    {
        printf("ERROR unknown command '%s'; enter 'help'\n", command.c_str());
    }
}

void usb_serial_commandline::print_help()
{
    printf("Commands (Wi-Fi protocol aliases are shown after '|'):\n");
    printf("  time|st HH:MM:SS              Set local clock time\n");
    printf("  date|sd DD.MM.YYYY             Set local clock date\n");
    printf("  timezone|tz [+HH:MM]           Set/show Wi-Fi time offset (-12:00..+14:00)\n");
    printf("  wifi-time-ignore|iwt [on|off]  Set/show whether Wi-Fi time is ignored\n");
    printf("  brightness|sb auto|10..255     Set automatic or manual brightness\n");
    printf("  brightness-curve|sbc 10..100   Set automatic brightness curve\n");
    printf("  faceplate|sfp NAME|0..6        Set language/faceplate\n");
    printf("  orientation|sdo normal|rotated Set display orientation\n");
    printf("  daylight-saving|dls on|off     Set EU daylight-saving adjustment\n");
    printf("  color-mode|col NAME|0..3       Set display color mode\n");
    printf("  dot-brightness|dob 0..100      Set minute-dot brightness\n");
    printf("  blink-dots|bld on|off          Set blinking minute dots\n");
    printf("  display-ip|ip A.B.C.D           Display an IPv4 address\n");
    printf("  restore-settings|res confirm   Restore all clock defaults\n");
    printf("  help                            Show this help\n");
}
