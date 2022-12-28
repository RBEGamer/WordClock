#ifndef __RTC_HPP__
#define __RTC_HPP__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"



#include "../helper.h"
#define RTC_TIME_DELIMITER ":"
#define RTC_DATE_DELIMITER "."

#define RTC_TIMEZONE_UTC 0
#define RTC_TIMEZONE_MEZ 1


#ifndef RTC_TIMEZONE
#define RTC_TIMEZONE RTC_TIMEZONE_UTC
#endif

#define RTC_LEAP_YEAR(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))

class rtc
{

protected:
    static bool enable_daylightsaving;

public:


    static int day_of_week(const int year, const signed char month, const signed char day);
    static int year_formatter(const int _year); // formats the year 2022 -> 22 1943 -> 43
    static bool summertime_eu(const int _year, const signed char _month, const signed char _day, const signed char _hour);
    static std::string get_compiletime_date();
    rtc();
    virtual ~rtc();

    virtual void set_rtc_time(const std::string _time, bool _initial);
    virtual void set_rtc_time(const signed char _h, const signed char _m, const signed char _s, bool _initial);
    virtual void set_rtc_date(const std::string _time);
    virtual void set_rtc_date(const signed char _day, const signed char _month, const int _year);
    virtual void init();
    void set_daylightsaving(bool _enable_daylightsaving);
    virtual datetime_t read_rtc();
    virtual void set_initial_time();

private:
    datetime_t current_time;
};
#endif