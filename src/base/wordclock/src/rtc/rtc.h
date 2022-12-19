#ifndef __RTC_H__
#define __RTC_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"



#include "../helper.h"
class rtc
{
public:

    rtc();
    virtual ~rtc();

    
    virtual void set_rtc_time(const std::string _time);
    virtual void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s);
    virtual void init_rtc();
    virtual datetime_t read_rtc();
private:
    datetime_t current_time;
};
#endif