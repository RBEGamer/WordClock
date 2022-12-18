#ifndef __RTC_H__
#define __RTC_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#ifdef USE_RP2040RTC
#include "hardware/rtc.h"
#endif

#include "../helper.h"
class rtc
{
public:

    rtc();
    virtual ~rtc();

    
    void set_rtc_time(const std::string _time);
    void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s);
    void init_rtc();
    datetime_t read_rtc();

};
#endif