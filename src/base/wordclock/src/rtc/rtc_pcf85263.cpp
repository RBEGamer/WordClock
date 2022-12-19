#include "rtc_pcf85263.h"



rtc_pcf85263::rtc_pcf85263(const int _pcf85263_i2c_addr){
    rtc_pcf85263::set_pcf85263_i2c_addr(_pcf85263_i2c_addr);
}


void rtc_pcf85263::set_pcf85263_i2c_addr(const int _pcf85263_i2c_addr){
    rtc_pcf85263::pcf85263_i2c_addr = _pcf85263_i2c_addr;
}


rtc_pcf85263::~rtc_pcf85263(){

}


void rtc_pcf85263::set_rtc_time(const std::string _time)
{
    if (_time.length() > 0)
    {
        const char *delim = ":";
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_time(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_pcf85263::set_rtc_time(const int8_t _h, const int8_t _m, const int8_t _s)
{
    datetime_t t = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 0, // 0 is Sunday, so 5 is Friday
        .hour = _h,
        .min = _m,
        .sec = _s};


    sleep_us(64);
    
}

void rtc_pcf85263::init_rtc()
{

}

datetime_t rtc_pcf85263::read_rtc()
{
    datetime_t t;

    return t;
}
