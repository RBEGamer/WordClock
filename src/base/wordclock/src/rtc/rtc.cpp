#include "rtc.h"

rtc::rtc()
{
}

rtc::~rtc()
{
}

void rtc::set_rtc_time(const std::string _time)
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

void rtc::set_rtc_time(const signed char _h, const signed char _m, const signed char _s)
{
    const datetime_t tmp = rtc::current_time;
    rtc::current_time = {
        .year = tmp.year,
        .month = tmp.month,
        .day = tmp.day,
        .dotw = tmp.dotw,
        .hour = _h,
        .min = _m,
        .sec = _s
    };
}



void rtc::set_rtc_date(const std::string _time)
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

void rtc::set_rtc_date(const signed char _day, const signed char _month, const int _year)
{
    const datetime_t tmp = rtc::current_time;

    rtc::current_time = {
        .year = (signed char)rtc::year_formatter(_year),
        .month = _month,
        .day = _day,
        .dotw = (signed char)rtc::day_of_week(_day, _month, _year), // 0 is Sunday, so 5 is Friday
        .hour = tmp.hour,
        .min = tmp.min,
        .sec = tmp.sec};
}

void rtc::init_rtc()
{
}

datetime_t rtc::read_rtc()
{
    return rtc::current_time;
}



int rtc::year_formatter(const int _year){
    if(_year > 9999){
        return _year % 1000;
    }else if(_year > 999){
        return _year % 100;
    }
    return _year;
}


// https://forum.arduino.cc/t/get-weekday-out-of-date/181165/7
int rtc::day_of_week(const int year, const signed char month, const signed char day)
{
    uint16_t months[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}; // days until 1st of month

    uint32_t days = year * 365; // days until year
    // adjust leap years, test only multiple of 4 of course
    for (uint16_t i = 4; i < year; i += 4)
    {
        if (RTC_LEAP_YEAR(i))
        {
            days++;
        }
    }

    days += months[month - 1] + day; // add the days of this year
    if ((month > 2) && RTC_LEAP_YEAR(year))
        days++; // adjust 1 if this year is a leap year, but only after febr

    return days % 7; // remove all multiples of 7
}