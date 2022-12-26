#ifndef __RTC_H__
#define __RTC_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "../helper.h"
#define RTC_TIME_DELIMITER ":"
#define RTC_DATE_DELIMITER "."

#define RTC_LEAP_YEAR(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))
class rtc
{
public:
    static int day_of_week(const int year, const signed char month, const signed char day);
    static int year_formatter(const int _year); // formats the year 2022 -> 22 1943 -> 43

    rtc();
    virtual ~rtc();

    virtual void set_rtc_time(const std::string _time);
    virtual void set_rtc_time(const signed char _h, const signed char _m, const signed char _s);
    virtual void set_rtc_date(const std::string _time);
    virtual void set_rtc_date(const signed char _day, const signed char _month, const int _year);
    virtual void init_rtc();
    virtual datetime_t read_rtc();

private:
    datetime_t current_time;
};
#endif