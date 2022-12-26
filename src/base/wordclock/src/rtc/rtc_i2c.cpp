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
        const char *delim = RTC_TIME_DELIMITER;
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_time(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_i2c::set_rtc_time(const signed char _h, const signed char _m, const signed char _s)
{
    // s, m, h, day, dayofweek sunday=0, month, year
    datetime_t t = read_rtc();
    uint8_t buf_tx[7] = {decToBcd(_s), decToBcd(_m), decToBcd(_h), decToBcd(t.dotw), decToBcd(t.day), decToBcd(t.month), decToBcd(t.year)};
    const int ret = helper::reg_write(i2c_default, RTC_I2C_ADDR, 0x01, buf_tx, (sizeof(buf_tx) / sizeof(uint8_t)));
}

void rtc_i2c::set_rtc_date(const std::string _time)
{
    if (_time.length() > 0)
    {
        const char *delim = RTC_DATE_DELIMITER;
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_date(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_i2c::set_rtc_date(const signed char _day, const signed char _month, const int _year)
{
    datetime_t t = read_rtc();
    const signed char dow = (signed char)rtc::day_of_week(_day, _month, _year);
    const signed char year = (signed char)rtc::year_formatter(_year);
    
    uint8_t buf_tx[7] = {decToBcd(t.sec), decToBcd(t.min), decToBcd(t.hour), decToBcd(dow), decToBcd(_day), decToBcd(_month), decToBcd(year)};
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
        .sec = (int8_t)bcdToDec(buf_rx[0] & 0b01111111)};

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
