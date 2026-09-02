#include "main.h"

int current_brightness = 10;
int current_brightness_mode = 0; // 0=auto >0-255 = manual
int last_brightness = 0;
int last_tmin = -1;
int last_tsec = -1;
long long last_update = 0;
bool display_update = true;
int brighness_curve = 10;
std::string display_to_ip = ""; // IF THERE IS SET ANYTHING THIS WILL BE SHOWN ON THE CLOCK
bool ignore_wifi_time = false;
int timezone_offset_minutes = 0;
int wifi_time_date_adjustment = 0;

namespace
{
constexpr int TIMEZONE_MINUTES_MIN = -12 * 60;
constexpr int TIMEZONE_MINUTES_MAX = 14 * 60;
constexpr int TIMEZONE_MINUTES_STEP = 15;
constexpr int TIMEZONE_STORAGE_BIAS = 49;
constexpr int TIMEZONE_STORAGE_MAX = (TIMEZONE_MINUTES_MAX / TIMEZONE_MINUTES_STEP) + TIMEZONE_STORAGE_BIAS;

uint8_t encode_timezone_offset(const int _offset_minutes)
{
    return (uint8_t)((_offset_minutes / TIMEZONE_MINUTES_STEP) + TIMEZONE_STORAGE_BIAS);
}

int decode_timezone_offset(const uint8_t _stored_value)
{
    // Zero and erased flash (0xff) both decode to UTC for backward compatibility.
    if (_stored_value < 1 || _stored_value > TIMEZONE_STORAGE_MAX)
    {
        return 0;
    }
    return ((int)_stored_value - TIMEZONE_STORAGE_BIAS) * TIMEZONE_MINUTES_STEP;
}

bool parse_unsigned_value(const std::string &_value, const int _minimum, const int _maximum, int &_result)
{
    if (_value.empty())
    {
        return false;
    }

    int result = 0;
    for (const char ch : _value)
    {
        if (ch < '0' || ch > '9')
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

bool extract_time(const std::string &_payload, int &_hour, int &_minute, int &_second)
{
    const size_t first_separator = _payload.find(':');
    const size_t second_separator = first_separator == std::string::npos
                                        ? std::string::npos
                                        : _payload.find(':', first_separator + 1);
    if (first_separator == std::string::npos ||
        second_separator == std::string::npos ||
        _payload.find(':', second_separator + 1) != std::string::npos)
    {
        return false;
    }

    return parse_unsigned_value(_payload.substr(0, first_separator), 0, 23, _hour) &&
           parse_unsigned_value(_payload.substr(first_separator + 1, second_separator - first_separator - 1), 0, 59, _minute) &&
           parse_unsigned_value(_payload.substr(second_separator + 1), 0, 59, _second);
}

bool extract_date(const std::string &_payload, int &_day, int &_month, int &_year)
{
    const size_t first_separator = _payload.find('.');
    const size_t second_separator = first_separator == std::string::npos
                                        ? std::string::npos
                                        : _payload.find('.', first_separator + 1);
    if (first_separator == std::string::npos ||
        second_separator == std::string::npos ||
        _payload.find('.', second_separator + 1) != std::string::npos)
    {
        return false;
    }

    return parse_unsigned_value(_payload.substr(0, first_separator), 1, 31, _day) &&
           parse_unsigned_value(_payload.substr(first_separator + 1, second_separator - first_separator - 1), 1, 12, _month) &&
           parse_unsigned_value(_payload.substr(second_separator + 1), 2000, 2099, _year);
}

int days_in_month(const int _month, const int _year)
{
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (_month == 2 && RTC_LEAP_YEAR(_year))
    {
        return 29;
    }
    return days[_month - 1];
}

void adjust_date(int &_day, int &_month, int &_year, const int _day_adjustment)
{
    _day += _day_adjustment;
    if (_day < 1)
    {
        _month--;
        if (_month < 1)
        {
            _month = 12;
            _year--;
        }
        _day = days_in_month(_month, _year);
    }
    else if (_day > days_in_month(_month, _year))
    {
        _day = 1;
        _month++;
        if (_month > 12)
        {
            _month = 1;
            _year++;
        }
    }
}
} // namespace

// CLASS INSTANCES
wordclock_faceplate *faceplate = new wordclock_faceplate();
ambient_light *lightsensor = nullptr;
rtc *timekeeper = nullptr;
settings_storage *settings = nullptr;

void switch_fp(wordclock_faceplate *&_instance, wordclock_faceplate::FACEPLATES _faceplate)
{

    if (_instance)
    {
        delete _instance;
    }
    wordclock_faceplate::config.current_faceplate = _faceplate;
    switch (_faceplate)
    {
    case wordclock_faceplate::FACEPLATES::TEST:
        _instance = new wordclock_faceplate();
        break;
    case wordclock_faceplate::FACEPLATES::GERMAN:
        _instance = new wordclock_faceplate_german();
        break;
    case wordclock_faceplate::FACEPLATES::BINARY:
        _instance = new wordclock_faceplate_binary();
        break;
    case wordclock_faceplate::FACEPLATES::ENGLISH:
        _instance = new wordclock_faceplate_english();
        break;
    case wordclock_faceplate::FACEPLATES::ITALIAN:
        _instance = new wordclock_faceplate_italian();
        break;
    case wordclock_faceplate::FACEPLATES::DOTS:
        _instance = new wordclock_faceplate_dots();
        break;
    default:
        _instance = new wordclock_faceplate();
        wordclock_faceplate::config.current_faceplate = wordclock_faceplate::FACEPLATES::TEST;
        break;
    }
}

void init_bh1750(const int _i2_addr)
{
    if (_i2_addr < 0)
    {
        printf("init_bh1750 failed");
        return;
    }

#ifdef USE_BH1750_IF_PRESENT
    if (lightsensor)
    {
        delete lightsensor;
    }
    lightsensor = new ambient_light_bh1750();
#endif
}

void init_eeprom_i2c(const int _i2_addr)
{
    if (_i2_addr < 0)
    {
        printf("init_m24c02 failed");
        return;
    }

#ifdef USE_EEPROM_IF_PRESENT
    if (settings)
    {
        delete settings;
    }
    settings = new settings_storage_eeprom();
#endif
}

void init_rtc_i2c(const int _i2_addr)
{
    if (_i2_addr < 0)
    {
        printf("init_pcf85263 failed");
        return;
    }
#ifdef USE_I2CRTC_IF_PRESENT
    if (timekeeper)
    {
        delete timekeeper;
    }
    timekeeper = new rtc_i2c();
#endif
}

void init_i2c()
{
    i2c_init(i2c_default, PICO_DEFAULT_I2C_SPEED); // 100khz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        int ret;
        uint8_t rxdata;
        if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78)
        {
            ret = PICO_ERROR_GENERIC;
        }
        else
        {
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
        }

        if (ret > 0)
        {
            printf("i2c device at %02x \n", addr);
        }

        // CHECK FOR REQUESTED DEVIES FOUND
        if (addr == BH1750_I2C_ADDR)
        {
            init_bh1750(addr);
        }
        else if (addr == RTC_I2C_ADDR)
        {
            init_rtc_i2c(addr);
        }
        else if (addr == EEPROM_I2C_ADDR)
        {
            init_eeprom_i2c(addr);
        }
    }
}

void update_display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    if (faceplate)
    {
        faceplate->display_time(_leds, _h, _m, _s);
    }
}

void display_ip(PicoLed::PicoLedController &_leds, const std::string _ip)
{
    const char *delim = ".";
    std::vector<std::string> out;
    helper::tokenize(_ip, delim, out);
    for (int i = 0; i < out.size(); i++)
    {
        // LIGHT UP FOR A IP BLOCK
        _leds.setBrightness(128);
        for (int j = 0; j < out.at(i).size(); j++)
        {
            if (faceplate)
            {
                const int ip_digit = (int)(out.at(i).at(j) - '0');
                faceplate->display_time(_leds, ip_digit, 0, (ip_digit * 1 / 255) % 60);
            }
            sleep_ms(1000);
        }
        // DARKEN = BETWEEN IP BLOCK
        _leds.setBrightness(10);
        sleep_ms(2000);
    }
    // RESTORE BRIGHTNESS
    _leds.setBrightness(last_brightness);
}

void set_brightnesmode(const std::string _payload)
{
    if (_payload.empty())
    {
        current_brightness_mode = 0;
    }
    current_brightness_mode = helper::limit(_payload, 0, 255);
    settings->write(settings_storage::SETTING_ENTRY::BRIGHTNESS, current_brightness_mode);
}

int set_faceplate(const int _fp)
{
    const int faceplate_index = helper::limit(_fp, 0, (int)wordclock_faceplate::FACEPLATES::TEST);
    switch_fp(faceplate, static_cast<wordclock_faceplate::FACEPLATES>(faceplate_index));
    return faceplate_index;
}

void set_faceplate_str(const std::string _payload)
{
    const int actual_fb = set_faceplate(helper::limit(_payload, 0, (int)wordclock_faceplate::FACEPLATES::TEST));
    settings->write(settings_storage::SETTING_ENTRY::FACEPLATE, actual_fb);
}

void set_displayorientation_str(const std::string _payload)
{
    wordclock_faceplate::config.flip_state = (bool)helper::limit(_payload, 0, 1);
    settings->write(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION, (int)wordclock_faceplate::config.flip_state);
}

void set_displayorientation(const int _payload)
{
    wordclock_faceplate::config.flip_state = (bool)helper::limit(_payload, 0, 1);
    settings->write(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION, (int)wordclock_faceplate::config.flip_state);
}

void set_time_from_wifi(const std::string _payload)
{
    if (ignore_wifi_time)
    {
        wifi_time_date_adjustment = 0;
        printf("wifi time update ignored\n");
        return;
    }

    int hour = 0;
    int minute = 0;
    int second = 0;
    if (!extract_time(_payload, hour, minute, second))
    {
        printf("invalid wifi time: %s\n", _payload.c_str());
        return;
    }

    int adjusted_minutes = (hour * 60) + minute + timezone_offset_minutes;
    wifi_time_date_adjustment = 0;
    if (adjusted_minutes < 0)
    {
        adjusted_minutes += 24 * 60;
        wifi_time_date_adjustment = -1;
    }
    else if (adjusted_minutes >= 24 * 60)
    {
        adjusted_minutes -= 24 * 60;
        wifi_time_date_adjustment = 1;
    }

    timekeeper->set_rtc_time(adjusted_minutes / 60, adjusted_minutes % 60, second, false);
}

void set_time_from_usb(const int _hour, const int _minute, const int _second)
{
    timekeeper->set_rtc_time(_hour, _minute, _second, false);
    display_update = true;
}

void set_date_from_wifi(const std::string _payload)
{
    int day = 0;
    int month = 0;
    int year = 0;
    if (!extract_date(_payload, day, month, year) || day > days_in_month(month, year))
    {
        printf("invalid wifi date: %s\n", _payload.c_str());
        return;
    }

    adjust_date(day, month, year, wifi_time_date_adjustment);
    timekeeper->set_rtc_date(day, month, year);
}

void set_date_from_usb(const std::string _payload)
{
    timekeeper->set_rtc_date(_payload);
    display_update = true;
}

void set_ignore_wifi_time(const bool _ignore)
{
    ignore_wifi_time = _ignore;
    settings->write(settings_storage::SETTING_ENTRY::IGNOREWIFITIME, (uint8_t)_ignore);
}

bool get_ignore_wifi_time()
{
    return ignore_wifi_time;
}

void set_timezone_offset(const int _offset_minutes)
{
    if (_offset_minutes < TIMEZONE_MINUTES_MIN ||
        _offset_minutes > TIMEZONE_MINUTES_MAX ||
        _offset_minutes % TIMEZONE_MINUTES_STEP != 0)
    {
        return;
    }
    timezone_offset_minutes = _offset_minutes;
    settings->write(settings_storage::SETTING_ENTRY::TIMEZONEOFFSET, encode_timezone_offset(_offset_minutes));
}

int get_timezone_offset()
{
    return timezone_offset_minutes;
}

void set_dls(const std::string _payload)
{
    const int v = (bool)helper::limit(_payload, 0, 1);
    timekeeper->set_daylightsaving(v);
    settings->write(settings_storage::SETTING_ENTRY::DAYLIGHTSAVING, v);
}

void set_dob(const std::string _payload)
{
    const int v = helper::limit(_payload, 0, 100);
    wordclock_faceplate::config.dotbrightness = v;
    settings->write(settings_storage::SETTING_ENTRY::DOTBRIGHTNESS, v);
}

void set_brightnesscurve(const std::string _payload)
{
    brighness_curve = helper::limit(_payload, 10, 100);
    settings->write(settings_storage::SETTING_ENTRY::BRIGHTNESSCURVE, brighness_curve);
}

void set_blinkendots(const std::string _payload)
{
    const bool v = (bool)helper::limit(_payload, 0, 1);
    wordclock_faceplate::config.blinkendots = v;
    settings->write(settings_storage::SETTING_ENTRY::BLINKENDOTS, v);
}

void set_colormode(const std::string _payload)
{
    const int colormode_index = helper::limit(_payload, 0, (int)wordclock_faceplate::COLORMODE::LENGTH);
    faceplate->set_colormode(static_cast<wordclock_faceplate::COLORMODE>(colormode_index));
    settings->write(settings_storage::SETTING_ENTRY::COLORMODE, (int)wordclock_faceplate::config.color_mode);
}

int apply_brightnesscurve(const int _in)
{
    // LINEAR IF brighness_curve <= 10
    if (brighness_curve < 11)
    {
        return _in;
    }
    // EXPONENTIAL IN 0.02 steps
    return helper::limit(std::pow(_in, 1 + (brighness_curve / 20.0)), WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN, WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX);
}

void prepare_display_ip(const std::string _payload)
{
    if (_payload.size() > 0)
    {
        display_to_ip = _payload;
    }
}

void restore_settings(bool _force = false)
{
    // USED TO CHECK VALUE OF SETTING_ENTRY::INVALID
    // IF VALUES ARENT MATCHING => ERASE FLASH/EEPROM
    // USE THE RP2040 CHIPID BECAUSE ITS DIFFERENT ON EACH BOARD SO A INITIAL WRITE WILL BE PERFORMED ON FIRST SW LOAD
    // ALSO ON EACH VERSION CHANGE OR PCB VERSION CHANGE A RESTORE SETTINGS WILL BE INITIATED USING VERSION AND WORDCLOCK_PCBREV DEFINED FROM CAMKE AND BOARD SETTINGS.h
    pico_unique_board_id_t chip_id;
    pico_get_unique_board_id(&chip_id);
    int versionint = 0;

#ifdef VERSION
    versionint = helper::version2int(VERSION);
#endif

#ifdef WORDCLOCK_PCBREV
    versionint += WORDCLOCK_PCBREV;
#endif

    const int check_value = (chip_id.id[0] + chip_id.id[7] + versionint) % 255;

    if (_force || settings->read(settings_storage::SETTING_ENTRY::INVALID) != check_value)
    {
        settings->write(settings_storage::SETTING_ENTRY::INVALID, check_value);
        // WRITE DEFAULT VALUES HERE
        // USED FROM THE BOARD SETTINGS
        settings->write(settings_storage::SETTING_ENTRY::FACEPLATE, WORDCLOCK_LANGUAGE);
        settings->write(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION, WORDCLOCK_DISPlAY_ORIENTATION);
        settings->write(settings_storage::SETTING_ENTRY::BRIGHTNESS, WORDCLOCK_BRIGHTNESS_MODE);
        settings->write(settings_storage::SETTING_ENTRY::DAYLIGHTSAVING, WORDCLOCK_DAYLIGHTSAVING);
        settings->write(settings_storage::SETTING_ENTRY::BRIGHTNESSCURVE, WORDCLOCK_BRIGHTNESS_MODE_AUTO_CURVE);
        settings->write(settings_storage::SETTING_ENTRY::COLORMODE, WORDCLOCK_COLOR_MODE);
        settings->write(settings_storage::SETTING_ENTRY::DOTBRIGHTNESS, WORDCLOCK_DOTBRIGHTNESS);
        settings->write(settings_storage::SETTING_ENTRY::BLINKENDOTS, WORDCLOCK_BLINKENDOTS);
        settings->write(settings_storage::SETTING_ENTRY::IGNOREWIFITIME, 0);
        settings->write(settings_storage::SETTING_ENTRY::TIMEZONEOFFSET, encode_timezone_offset(0));
        // SET THE RTC TO A DEFINED TIME
        if (timekeeper)
        {
            timekeeper->set_initial_time();
        }
    }

    // LOAD VALUES FROM STORAGE
    current_brightness_mode = settings->read(settings_storage::SETTING_ENTRY::BRIGHTNESS);
    set_faceplate(settings->read(settings_storage::SETTING_ENTRY::FACEPLATE));
    wordclock_faceplate::config.flip_state = (bool)helper::limit(settings->read(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION), 0, 1);
    timekeeper->set_daylightsaving((bool)helper::limit(settings->read(settings_storage::SETTING_ENTRY::DAYLIGHTSAVING), 0, 1));
    brighness_curve = settings->read(settings_storage::SETTING_ENTRY::BRIGHTNESSCURVE);
    faceplate->set_colormode(static_cast<wordclock_faceplate::COLORMODE>(settings->read(settings_storage::SETTING_ENTRY::COLORMODE)));
    wordclock_faceplate::config.dotbrightness = settings->read(settings_storage::SETTING_ENTRY::DOTBRIGHTNESS);
    wordclock_faceplate::config.blinkendots = settings->read(settings_storage::SETTING_ENTRY::BLINKENDOTS);
    // Only 1 means enabled so erased/uninitialized storage (0xff) remains backward compatible.
    ignore_wifi_time = settings->read(settings_storage::SETTING_ENTRY::IGNOREWIFITIME) == 1;
    timezone_offset_minutes = decode_timezone_offset(settings->read(settings_storage::SETTING_ENTRY::TIMEZONEOFFSET));
}

void set_restoresettings(const std::string _payload)
{
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    restore_settings(true);
    sleep_ms(100);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
}

int main()
{
    bool led_state = false;
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    wordclock_faceplate::config = wordclock_faceplate::FACEPLATE_CONFIG();

    init_i2c();
    // AFTER CALLING init_i2c all needed instances of timekeeper lightsensor settings objects should be created.
    //  IF A I2C DEVICE ISNT PRESENT CREATE DEFAULT ONES
    if (!settings)
    {
#if defined(USE_FLASH_AS_EEPROM)
        settings = new settings_storage_flash();
#else
        settings = new settings_storage();
#endif
    }

    if (!timekeeper)
    {
#if defined(USE_RP2040RTC)
        timekeeper = new rtc_rp2040();
#else
        timekeeper = new rtc();
#endif
    }

    if (!lightsensor)
    {
        lightsensor = new ambient_light();
    }

    if (!settings || !timekeeper || !lightsensor)
    {
        while (1)
        {
            printf("init failed: settings_%i timekeeper_%i lightsensor_%i\n", settings, timekeeper, lightsensor);
            led_state = !led_state;
            gpio_put(PICO_DEFAULT_LED_PIN, led_state);
            sleep_ms(100);
        }
    }

    settings->init();
    timekeeper->init();
    lightsensor->init();

    // modified lib for 400khz
    PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, PICO_DEFAULT_WS2812_PIN, PICO_DEFAULT_WS2812_NUM, PicoLed::FORMAT_GRB);
    // set initial brightness
    ledStrip.clear();
    ledStrip.show();
    ledStrip.setBrightness(128);
    sleep_ms(500);
    // DISPLAY TESTPATTERN => light up all corners to test matrix settings
    faceplate->display_testpattern(ledStrip);
    sleep_ms(1000 + DEBUG * 5000);
    // current_brightness = lightsensor->get_brightness();
    // ledStrip.setBrightness(current_brightness);

    // enable uart rx irq for communication with wifi module and register callback functions
    wifi_interface::init_uart();
#ifdef ENABLE_UART_IRQ
    wifi_interface::enable_uart_irq(true);
#endif
    wifi_interface::register_rx_callback(set_time_from_wifi, wifi_interface::CMD_INDEX::TIME);
    wifi_interface::register_rx_callback(set_brightnesmode, wifi_interface::CMD_INDEX::BRIGHTNESS);
    wifi_interface::register_rx_callback(set_faceplate_str, wifi_interface::CMD_INDEX::FACEPLATE);
    wifi_interface::register_rx_callback(prepare_display_ip, wifi_interface::CMD_INDEX::DISPLAYIP);
    wifi_interface::register_rx_callback(set_displayorientation_str, wifi_interface::CMD_INDEX::DISPLAYORIENTATION);
    wifi_interface::register_rx_callback(set_dls, wifi_interface::CMD_INDEX::DAYLIGHTSAVING);
    wifi_interface::register_rx_callback(set_brightnesscurve, wifi_interface::CMD_INDEX::BRIGHTNESSCURVE);
    wifi_interface::register_rx_callback(set_date_from_wifi, wifi_interface::CMD_INDEX::DATE);
    wifi_interface::register_rx_callback(set_colormode, wifi_interface::CMD_INDEX::COLORMODE);
    wifi_interface::register_rx_callback(set_restoresettings, wifi_interface::CMD_INDEX::RESTORESETTINGS);
    wifi_interface::register_rx_callback(set_dob, wifi_interface::CMD_INDEX::DOTBRIGHTNESS);
    wifi_interface::register_rx_callback(set_blinkendots, wifi_interface::CMD_INDEX::BLINKENDOTS);

// RESTORE ALLE SETTINGS
//  DO ITS AT THE END (AFTER I2C INIT ) -> settings source could changesd to eeprom if enabled
#ifdef FORCE_RESTORE_SETTINGS
    restore_settings(true);
#else
    restore_settings(false);
#endif

    usb_serial_commandline::callbacks usb_callbacks;
    usb_callbacks.set_time = set_time_from_usb;
    usb_callbacks.set_date = set_date_from_usb;
    usb_callbacks.set_brightness = set_brightnesmode;
    usb_callbacks.display_ip = prepare_display_ip;
    usb_callbacks.set_faceplate = set_faceplate_str;
    usb_callbacks.set_orientation = set_displayorientation_str;
    usb_callbacks.set_brightness_curve = set_brightnesscurve;
    usb_callbacks.set_daylight_saving = set_dls;
    usb_callbacks.set_color_mode = set_colormode;
    usb_callbacks.restore_settings = set_restoresettings;
    usb_callbacks.set_dot_brightness = set_dob;
    usb_callbacks.set_blink_dots = set_blinkendots;
    usb_callbacks.set_ignore_wifi_time = set_ignore_wifi_time;
    usb_callbacks.get_ignore_wifi_time = get_ignore_wifi_time;
    usb_callbacks.set_timezone_offset = set_timezone_offset;
    usb_callbacks.get_timezone_offset = get_timezone_offset;
    usb_serial_commandline::init(usb_callbacks);

    gpio_put(PICO_DEFAULT_LED_PIN, false);


    
    while (true)
    {
        sleep_ms(10);

        usb_serial_commandline::process();

// PROCEESS ANY RECEIEVED COMMANDS
#ifndef ENABLE_UART_IRQ
        wifi_interface::on_wifi_uart_rx();
#endif
        wifi_interface::process_cmd();
        // SET UPDATE DISPLAY EVERY x00 MS
        const long long current_update = time_us_64();
        if (abs(last_update - current_update) > (300 * 1000))
        {
            last_update = current_update;
            display_update = true;
        }

        // UPDATE DISPLAY STUFF HERE
        if (display_update)
        {
            display_update = false;
            // DISPLAY IP IF NEEDED
            if (display_to_ip.size() > 0)
            {
                display_ip(ledStrip, display_to_ip);
                display_to_ip = "";
            }
            // UPDATE DISPLAY IF NEEDED
            const datetime_t t = timekeeper->read_rtc();
            if (last_tsec != t.sec)
            {
                last_tsec = t.sec;
                printf("h%i m%i s%i b%i d%i m%i y%i\n", t.hour, t.min, t.sec, current_brightness, t.day, t.month, t.year);
                update_display_time(ledStrip, t.hour, t.min, t.sec);
            }
            // CHECK BRIGHTNESS
            if (current_brightness_mode == 0)
            {
                const int sensor_read = apply_brightnesscurve(lightsensor->get_average_brightness()); // automatic if mode = 0
                if (sensor_read > current_brightness && current_brightness <= 255)
                {
                    current_brightness++;
                }
                else if (sensor_read < current_brightness && current_brightness > 11)
                {
                    current_brightness--;
                }
            }
            else
            {
                current_brightness = current_brightness_mode; // manual if mode 1-255
            }
            // UPDATE BRIGHTNESS IF NEEDED
            if (abs(current_brightness - last_brightness) > 0)
            {
                last_brightness = current_brightness;
                ledStrip.setBrightness(current_brightness);
#ifdef ENABLE_SMMOTH_FADE
                // UPDATE DISPLAY AGAIN TO APPLY NEW BRIGHTNESS
                update_display_time(ledStrip, t.hour, t.min, t.sec);
#endif
            }
        }
    }
    return 0;
}
