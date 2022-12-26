#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#include "hardware/flash.h" // for the flash erasing and writing
#include "hardware/sync.h"  // for the interrupts

#include <PicoLed.hpp>

#include "wifi_interface.h"
#include "settings_storage/settings_storage_include.h"
#include "helper.h"
#include "rtc/rtc_include.h"
#include "wordclock_faceplate/wordclock_faceplate_include.h"
#include "ambient_light/ambient_light_include.h"


int current_brightness = 10;
int current_brightness_mode = 0; // 0=auto >0-255 = manual
int last_brightness = 0;
int last_tmin = -1;
int last_tsec = -1;



std::string display_to_ip = ""; // IF THERE IS SET ANYTHING THIS WILL BE SHOWN ON THE CLOCK
wordclock_faceplate *faceplate = new wordclock_faceplate();

#if defined(USE_FLASH_AS_EEPROM)
settings_storage *settings = new settings_storage_flash();
#else
settings_storage *settings = new settings_storage();
#endif

#if defined(USE_RP2040RTC)
rtc *timekeeper = new rtc_rp2040();
#else
rtc *timekeeper = new rtc();
#endif

ambient_light* light_sensor = new ambient_light();


void switch_fp(wordclock_faceplate *_instance, wordclock_faceplate::FACEPLATES _faceplate)
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

    if(light_sensor){
        delete light_sensor;
    }
    light_sensor = new ambient_light_bh1750();
    light_sensor->init();

}

void init_eeprom_i2c(const int _i2_addr)
{
    if (_i2_addr < 0)
    {
        printf("init_m24c02 failed");
        return;
    }

#ifdef USE_EEPROM_IF_EEPROM_IS_PRESENT
    if (settings)
    {
        delete settings;
    }
    settings = new settings_storage_eeprom();
    settings->init();
#endif
}

void init_rtc_i2c(const int _i2_addr)
{
    if (_i2_addr < 0)
    {
        printf("init_pcf85263 failed");
        return;
    }
#ifdef USE_RTC_IF_RTC_IS_PRESENT
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
            gpio_put(PICO_DEFAULT_LED_PIN, false);
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
        printf("display_ip %s", out.at(i).c_str());

        _leds.setBrightness(128);
        for (int j = 0; j < out.at(i).size(); j++)
        {
            if (faceplate)
            {
                faceplate->display_time(_leds, (int)(out.at(i).at(j) - '0'), 0, 0);
            }
            sleep_ms(1000);
        }
        _leds.setBrightness(10);
        sleep_ms(2000);
    }
}

void set_brightnesmode(const std::string _payload)
{
    printf("called set_brightnesmode");
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

void set_time(const std::string _payload)
{
    timekeeper->set_rtc_time(_payload);
}

void set_date(const std::string _payload)
{
    timekeeper->set_rtc_date(_payload);
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
    const int FLASH_CHECK_VALUE_START = 20;
    if (_force || settings->read(settings_storage::SETTING_ENTRY::INVALID) != FLASH_CHECK_VALUE_START);//|| settings->read(settings_storage::SETTING_ENTRY::LENGHT) != FLASH_CHECK_VALUE_END)
    {
        settings->write(settings_storage::SETTING_ENTRY::INVALID, FLASH_CHECK_VALUE_START);
        // WRITE DEFAULT VALUES HERE
        // USED FROM THE BOARD SETTINGS
        settings->write(settings_storage::SETTING_ENTRY::FACEPLATE, WORDCLOCK_LANGUAGE);
        settings->write(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION, WORDCLOCK_DISPlAY_ORIENTATION);
        settings->write(settings_storage::SETTING_ENTRY::BRIGHTNESS, WORDCLOC_BRIGHTNESS_MODE);
    }

    // LOAD VALUES FROM STORAGE
    current_brightness_mode = settings->read(settings_storage::SETTING_ENTRY::BRIGHTNESS);
    set_faceplate(settings->read(settings_storage::SETTING_ENTRY::FACEPLATE));
    wordclock_faceplate::config.flip_state = (bool)helper::limit(settings->read(settings_storage::SETTING_ENTRY::DISPLAYORIENTATION), 0, 1);
}

int main()
{
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    settings->init();
    timekeeper->init_rtc();
    light_sensor->init();

    sleep_ms(3000); // WAIT A BIT FOR UART/USB

    init_i2c();
   

    // modified lib for 400khz
    PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, PICO_DEFAULT_WS2812_PIN, PICO_DEFAULT_WS2812_NUM, PicoLed::FORMAT_GRB);
    // set initial brightness
    ledStrip.setBrightness(128);
    // DISPLAY TESTPATTERN => light up all corners to test matrix settings
    faceplate->display_testpattern(ledStrip);
    sleep_ms(1000);
    ledStrip.setBrightness(light_sensor->get_average_brightness());

    // enable uart rx irq for communication with wifi module and register callback functions
    wifi_interface::init_uart();
    wifi_interface::enable_uart_irq(true);
    wifi_interface::register_rx_callback(set_time, wifi_interface::CMD_INDEX::TIME);
    wifi_interface::register_rx_callback(set_brightnesmode, wifi_interface::CMD_INDEX::BRIGHTNESS);
    wifi_interface::register_rx_callback(set_faceplate_str, wifi_interface::CMD_INDEX::FACEPLATE);
    wifi_interface::register_rx_callback(prepare_display_ip, wifi_interface::CMD_INDEX::DISPLAYIP);
    wifi_interface::register_rx_callback(set_displayorientation_str, wifi_interface::CMD_INDEX::DISPLAYORIENTATION);
    wifi_interface::register_rx_callback(set_date, wifi_interface::CMD_INDEX::DATE);

    // RESTORE ALLE SETTINGS
    //  DO ITS AT THE END (AFTER I2C INIT ) -> settings source could changesd to eeprom if enabled
    restore_settings(false);
    wifi_interface::send_log("setupcomplete");
    //gpio_put(PICO_DEFAULT_LED_PIN, false);
  
    while (true)
    {
        // PROCEESS ANY RECEIEVED COMMANDS
        wifi_interface::process_cmd();

        if (display_to_ip.size() > 0)
        {
            display_ip(ledStrip, display_to_ip);
            display_to_ip = "";
        }

        // UPDATE DISPLAY IF NEEDED
        datetime_t t = timekeeper->read_rtc();
        if (last_tsec != t.sec)
        {
            last_tsec = t.sec;
            update_display_time(ledStrip, t.hour, t.min, t.sec);
        }
        // SEND TIME TO WIFI MODULE IF NEEDED
        if (last_tmin != t.min)
        {
            last_tmin = t.min;
            wifi_interface::send_time(t.hour, t.min, t.sec);
        }

        // UPDATE BRIGHTNESS IF NEEDED
        if (current_brightness_mode == 0)
        {
            current_brightness = light_sensor->get_average_brightness(); // automatic if mode = 0
        }
        else
        {
            current_brightness = current_brightness_mode; // manual if mode 1-255
        }

        if (abs(current_brightness - last_brightness) > 1)
        {
            last_brightness = current_brightness;
            wifi_interface::send_brightness(current_brightness);
            ledStrip.setBrightness(current_brightness);
        }

        sleep_ms(200);
    }

    return 0;
}
