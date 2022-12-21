#include "rtc.h"

rtc::rtc(){
    rtc::init_rtc();
    
}

rtc::~rtc(){
}


void rtc::set_rtc_time(const std::string _time)
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

void rtc::set_rtc_time(const uint8_t _h, const uint8_t _m, const uint8_t _s)
{
     rtc::current_time = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 0, // 0 is Sunday, so 5 is Friday
        .hour = _h,
        .min = _m,
        .sec = _s};
}

void rtc::init_rtc()
{
}

datetime_t rtc::read_rtc()
{
    return rtc::current_time;
}
