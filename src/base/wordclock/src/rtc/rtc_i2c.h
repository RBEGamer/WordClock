#ifndef __RTC_I2C_H__
#define __RTC_I2C_H__

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

#include "../helper.h"
#include "rtc.h"

class rtc_i2c: public rtc
{

private:
    uint8_t bcdToDec(const uint8_t _value);
    uint8_t decToBcd(const uint8_t _value);

public:

    rtc_i2c();
    ~rtc_i2c();

    void set_rtc_time(const std::string _time) override;
    void set_rtc_time(const signed char _h, const signed char _m, const signed char _s) override;
    void set_rtc_date(const std::string _time) override;
    void set_rtc_date(const signed char _day, const signed char _month, const int _year) override;
    void init_rtc() override;
    datetime_t read_rtc() override;


};
#endif