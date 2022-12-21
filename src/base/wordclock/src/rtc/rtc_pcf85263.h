#ifndef __RTC_PCF85263_H__
#define __RTC_PCF85263_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "../helper.h"
#include "rtc.h"

class rtc_pcf85263: public rtc
{
public:

    rtc_pcf85263(const int _pcf85263_i2c_addr);
    ~rtc_pcf85263();

    void set_rtc_time(const std::string _time) override;
    void set_rtc_time(const uint8_t _h, const uint8_t _m, const uint8_t _s) override;
    void init_rtc() override;
    datetime_t read_rtc() override;

private:

    int pcf85263_i2c_addr = 0x51;

    void set_pcf85263_i2c_addr(const int _pcf85263_i2c_addr);
    uint8_t bcdToDec(const uint8_t _value);
    uint8_t decToBcd(const uint8_t _value);
};
#endif