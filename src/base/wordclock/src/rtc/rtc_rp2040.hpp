#ifndef __RTC_RP2040_HPP__
#define __RTC_RP2040_HPP__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

#include "../helper.h"
#include "rtc.hpp"

class rtc_rp2040: public rtc
{
public:

    rtc_rp2040();
    ~rtc_rp2040();

    void set_rtc_time(const std::string _time) override;
    void set_rtc_time(const signed char _h, const signed char _m, const signed char _s) override;
    void set_rtc_date(const std::string _time) override;
    void set_rtc_date(const signed char _day, const signed char _month, const int _year) override;
    void init_rtc() override;
    datetime_t read_rtc() override;

};
#endif