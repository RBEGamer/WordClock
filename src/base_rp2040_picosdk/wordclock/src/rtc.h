#ifndef __RTC_H__
#define __RTC_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"


class rtc
{
public:
    static void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s);
    static void init_rtc();
    static datetime_t read_rtc();

};
#endif