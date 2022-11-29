#ifndef __RTC_H__
#define __RTC_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#ifdef USE_RP2040RTC
#include "hardware/rtc.h"
#endif

#include "helper.h"
class rtc
{
public:
    static void set_rtc_time(const std::string _time);
    static void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s);
    static void init_rtc();
    static datetime_t read_rtc();

};
#endif