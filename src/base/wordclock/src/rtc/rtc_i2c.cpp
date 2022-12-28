#include "rtc_i2c.hpp"

rtc_i2c::rtc_i2c() : rtc()
{
}

rtc_i2c::~rtc_i2c()
{
}

void rtc_i2c::set_rtc_time(const std::string _time, bool _initial)
{
    if (_time.length() > 0)
    {
        const char *delim = RTC_TIME_DELIMITER;
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_time(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()), _initial);
        }
    }
}

void rtc_i2c::set_rtc_time(const signed char _h, const signed char _m, const signed char _s, bool _initial)
{
    // s, m, h, day, dayofweek sunday=0, month, year
    if (_initial)
    {
        uint8_t buf_tx[7] = {decToBcd(_s), decToBcd(_m), decToBcd(_h), decToBcd(6), decToBcd(1), decToBcd(1), decToBcd(22)};
        const int ret = helper::reg_write(i2c_default, RTC_I2C_ADDR, 0x01, buf_tx, (sizeof(buf_tx) / sizeof(uint8_t)));
    }
    else
    {
        datetime_t t = read_rtc();
        uint8_t buf_tx[7] = {decToBcd(_s), decToBcd(_m), decToBcd(_h), decToBcd(t.dotw), decToBcd(t.day), decToBcd(t.month), decToBcd(t.year)};
        const int ret = helper::reg_write(i2c_default, RTC_I2C_ADDR, 0x01, buf_tx, (sizeof(buf_tx) / sizeof(uint8_t)));
    }
}

void rtc_i2c::set_rtc_date(const std::string _time){
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

void rtc_i2c::set_initial_time()
{
#ifdef SET_INITIAL_TIME_ZERO
    rtc_i2c::set_rtc_time(0, 0, 0, true);
    rtc_i2c::set_rtc_date(1, 1, 22);
#else
    rtc_i2c::set_rtc_time(__TIME__, true);
    rtc_i2c::set_rtc_date(rtc::get_compiletime_date());
#endif
}

void rtc_i2c::init()
{
}

datetime_t rtc_i2c::read_rtc_raw()
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

datetime_t rtc_i2c::read_rtc()
{
    datetime_t t = rtc_i2c::read_rtc_raw();
    if (rtc::enable_daylightsaving && !rtc::summertime_eu(t.year, t.month, t.day, t.hour))
    {
        signed char tmp = t.hour - 1;
        if (tmp < 0)
        {
            tmp += 24;
        }
        t.hour = tmp % 24;
    }
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
