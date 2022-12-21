#include "rtc_pcf85263.h"

rtc_pcf85263::rtc_pcf85263(const int _pcf85263_i2c_addr)
{
    //rtc_pcf85263::set_pcf85263_i2c_addr(_pcf85263_i2c_addr);
   
}

void rtc_pcf85263::set_pcf85263_i2c_addr(const int _pcf85263_i2c_addr)
{
    rtc_pcf85263::pcf85263_i2c_addr = _pcf85263_i2c_addr;
}

rtc_pcf85263::~rtc_pcf85263()
{
}

void rtc_pcf85263::set_rtc_time(const std::string _time)
{
    if (_time.length() > 0)
    {
        const char *delim = ":";
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_time(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_pcf85263::set_rtc_time(const uint8_t _h, const uint8_t _m, const uint8_t _s)
{
    // s, m, h, day, dayofweek sunday=0, month, year
    uint8_t buf_tx[7] = {decToBcd(_s), decToBcd(_m), decToBcd(_h), decToBcd(1), decToBcd(2), decToBcd(1), decToBcd(2020)};
    const int ret = helper::reg_write(i2c_default, rtc_pcf85263::pcf85263_i2c_addr, 0x01, buf_tx, (sizeof(buf_tx) / sizeof(uint8_t)));
}

void rtc_pcf85263::init_rtc()
{
   // gpio_put(PICO_DEFAULT_LED_PIN, false);
}

datetime_t rtc_pcf85263::read_rtc()
{
   // gpio_put(PICO_DEFAULT_LED_PIN, false);

    uint8_t buf_rx[7] = {0};
    helper::reg_read(i2c_default, rtc_pcf85263::pcf85263_i2c_addr, 0x01, buf_rx, (sizeof(buf_rx) / sizeof(uint8_t)));

    datetime_t t = {
        .year = bcdToDec(buf_rx[6]),
        .month = bcdToDec(buf_rx[5] & 0b00011111),
        .day = bcdToDec(buf_rx[3] & 0b00111111),
        .dotw = bcdToDec(buf_rx[4] & 0b00000111), // 0 is Sunday, so 5 is Friday
        .hour = bcdToDec(buf_rx[2] & 0b00111111),
        .min = bcdToDec(buf_rx[1] & 0b01111111),
        .sec =  bcdToDec(buf_rx[0] & 0b01111111)};
    return t;
}

uint8_t rtc_pcf85263::bcdToDec(const uint8_t _value)
{
    return ((_value / 16) * 10 + _value % 16);
}

uint8_t rtc_pcf85263::decToBcd(const uint8_t _value)
{
    return (_value / 10 * 16 + _value % 10);
}