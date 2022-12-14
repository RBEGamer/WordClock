#include "rtc.hpp"

bool rtc::enable_daylightsaving = WORDCLOCK_DAYLIGHTSAVING;

rtc::rtc()
{
}

rtc::~rtc()
{
}

void rtc::set_rtc_time(const std::string _time, bool _initial)
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

void rtc::set_rtc_time(const signed char _h, const signed char _m, const signed char _s, bool _initial)
{
    const datetime_t tmp = rtc::current_time;
    if (_initial)
    {
        rtc::current_time = {
            .year = 22,
            .month = 1,
            .day = 1,
            .dotw = 6,
            .hour = _h,
            .min = _m,
            .sec = _s};
    }
    else
    {
        rtc::current_time = {
            .year = tmp.year,
            .month = tmp.month,
            .day = tmp.day,
            .dotw = tmp.dotw,
            .hour = _h,
            .min = _m,
            .sec = _s};
    }
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

void rtc::set_initial_time()
{
#ifdef SET_INITIAL_TIME_ZERO
    rtc::set_rtc_time(0, 0, 0, true);
    rtc::set_rtc_date(1, 1, 22);
#else
    rtc::set_rtc_time(__TIME__, true);
    rtc::set_rtc_date(rtc::get_compiletime_date());
#endif
}

void rtc::init()
{
    rtc::set_initial_time();
}

void rtc::set_daylightsaving(bool _enable_daylightsaving)
{
    rtc::enable_daylightsaving = _enable_daylightsaving;
}

datetime_t rtc::read_rtc()
{
    datetime_t t = rtc::current_time;
    if (rtc::enable_daylightsaving && rtc::summertime_eu(t.year, t.month, t.day, t.hour))
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

int rtc::year_formatter(const int _year)
{
    if (_year > 9999)
    {
        return _year % 1000;
    }
    else if (_year > 999)
    {
        return _year % 100;
    }
    return _year;
}

// https://forum.arduino.cc/t/get-weekday-out-of-date/181165/7
int rtc::day_of_week(const int year, const signed char month, const signed char day)
{
    const uint16_t months[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}; // days until 1st of month
    uint32_t days = year * 365;                                                             // days until year
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
    {
        days++; // adjust 1 if this year is a leap year, but only after febr
    }
    return days % 7; // remove all multiples of 7
}

// https://forum.arduino.cc/t/time-libary-sommerzeit-winterzeit/221884/2
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
// #define RTC_TIMEZONE 0=UTC ,1=MEZ
bool rtc::summertime_eu(const int _year, const signed char _month, const signed char _day, const signed char _hour)
{
    if (_month < 3 || _month > 10)
    {
        return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
    }
    else if (_month > 3 && _month < 10)
    {
        return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
    }
    else if (_month == 3 && (_hour + 24 * _day) >= (1 + RTC_TIMEZONE + 24 * (31 - (5 * _year / 4 + 4) % 7)) || _month == 10 && (_hour + 24 * _day) < (1 + RTC_TIMEZONE + 24 * (31 - (5 * _year / 4 + 1) % 7)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string rtc::get_compiletime_date()
{
    const char *delim = " ";
    const std::string date = __DATE__;
    std::vector<std::string> out;
    helper::tokenize(date, delim, out);

    if (out.size() > 2)
    {
        int month = 0;
        const std::string ms = out.at(0);
        if (ms == "Jan")
        {
            month = 1;
        }
        else if (ms == "Feb")
        {
            month = 2;
        }
        else if (ms == "Mar")
        {
            month = 3;
        }
        else if (ms == "Apr")
        {
            month = 4;
        }
        else if (ms == "May")
        {
            month = 5;
        }
        else if (ms == "Jun")
        {
            month = 6;
        }
        else if (ms == "Jul")
        {
            month = 7;
        }
        else if (ms == "Aug")
        {
            month = 8;
        }
        else if (ms == "Sep")
        {
            month = 9;
        }
        else if (ms == "Oct")
        {
            month = 10;
        }
        else if (ms == "Nov")
        {
            month = 11;
        }
        else if (ms == "Dec")
        {
            month = 12;
        }
        return out.at(1) + ":" + std::to_string(month) + ":" + out.at(2);
    }

    return "1:1:1900";
}