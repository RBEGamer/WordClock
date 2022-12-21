#ifndef __RTC_PCF85263_H__
#define __RTC_PCF85263_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/i2c.h"

#include "../helper.h"
#include "rtc.h"

class rtc_pcf85263: public rtc
{
public:


    int pcf85263_i2c_addr = 0x51;


    rtc_pcf85263(const int _pcf85263_i2c_addr);
    virtual ~rtc_pcf85263();

    void set_pcf85263_i2c_addr(const int _pcf85263_i2c_addr);
    void set_rtc_time(const std::string _time) override;
    void set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s) override;
    void init_rtc() override;
    datetime_t read_rtc() override;



};
#endif