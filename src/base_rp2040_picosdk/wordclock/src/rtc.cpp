#include "rtc.h"


void rtc::set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s)
{
    datetime_t t = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 0, // 0 is Sunday, so 5 is Friday
        .hour = _h,
        .min = _m,
        .sec = _s};
    rtc_set_datetime(&t);
    sleep_us(64);
}

void rtc::init_rtc()
{
    rtc_init();
}

datetime_t rtc::read_rtc()
{
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}
