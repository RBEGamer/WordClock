#include "rtc_i2c.h"

rtc_i2c::rtc_i2c()
{
}

rtc_i2c::~rtc_i2c()
{
}



void rtc_i2c::set_rtc_time(const std::string _time)
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

void rtc_i2c::set_rtc_time(const uint8_t _h, const uint8_t _m, const uint8_t _s)
{
   // s, m, h, day, dayofweek sunday=0, month, year
    uint8_t buf_tx[7] = {decToBcd(_s), decToBcd(_m), decToBcd(_h), decToBcd(1), decToBcd(2), decToBcd(1), decToBcd(20)};
    const int ret = helper::reg_write(i2c_default, RTC_I2C_ADDR, 0x01, buf_tx, (sizeof(buf_tx) / sizeof(uint8_t)));
}

void rtc_i2c::init_rtc()
{
}

datetime_t rtc_i2c::read_rtc()
{
    uint8_t buf_rx[7] = {0};
    helper::reg_read(i2c_default, RTC_I2C_ADDR, 0x01, buf_rx, (sizeof(buf_rx) / sizeof(uint8_t)));

    datetime_t t = {
        .year = (int8_t)bcdToDec(buf_rx[6]),
        .month = (int8_t)bcdToDec(buf_rx[5] & 0b00011111),
        .day = (int8_t)bcdToDec(buf_rx[3] & 0b00111111),
        .dotw = (int8_t)bcdToDec(buf_rx[4] & 0b00000111), // 0 is Sunday, so 5 is Friday
        .hour = (int8_t)bcdToDec(buf_rx[2] & 0b00111111),
        .min = (int8_t)bcdToDec(buf_rx[1] & 0b01111111),
        .sec =  (int8_t)bcdToDec(buf_rx[0] & 0b01111111)};

    return t;
}



uint8_t rtc_i2c::bcdToDec(const uint8_t _value)
{
    return ((_value / 16) * 10 + _value % 16);
}

uint8_t rtc_i2c::decToBcd(const uint8_t _value)
{
    return (_value / 10 * 16 + _value % 10);
}
