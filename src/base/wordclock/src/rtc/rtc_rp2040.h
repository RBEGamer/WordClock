#ifndef __RTC_RP2040_H__
#define __RTC_RP2040_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

#include "../helper.h"
#include "rtc.h"

class rtc_rp2040: public rtc
{
public:

    rtc_rp2040();
    virtual ~rtc_rp2040();

    
    void set_rtc_time(const std::string _time) override;
    void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s) override;
    void init_rtc() override;
    datetime_t read_rtc() override;

};
#endif