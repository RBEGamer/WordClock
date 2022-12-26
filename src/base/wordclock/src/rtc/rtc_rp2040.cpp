#include "rtc_rp2040.hpp"

rtc_rp2040::rtc_rp2040()
{
}

rtc_rp2040::~rtc_rp2040()
{
}

void rtc_rp2040::set_rtc_time(const std::string _time)
{
    if (_time.length() > 0)
    {
        const char *delim = RTC_TIME_DELIMITER;
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_time(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_rp2040::set_rtc_time(const signed char _h, const signed char _m, const signed char _s)
{
    datetime_t t = read_rtc();
    t = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 0, // 0 is Sunday, so 5 is Friday
        .hour = _h,
        .min = _m,
        .sec = _s};
    rtc_set_datetime(&t);
    sleep_us(64);
}



void rtc_rp2040::set_rtc_date(const std::string _time)
{
    if (_time.length() > 0)
    {
        const char *delim = RTC_DATE_DELIMITER;
        std::vector<std::string> out;
        helper::tokenize(_time, delim, out);
        if (out.size() > 2)
        {
            set_rtc_date(std::atoi(out.at(0).c_str()), std::atoi(out.at(1).c_str()), std::atoi(out.at(2).c_str()));
        }
    }
}

void rtc_rp2040::set_rtc_date(const signed char _day, const signed char _month, const int _year)
{
    datetime_t t = read_rtc();
    t = {
        .year = (signed char)rtc::year_formatter(_year),
        .month = _month,
        .day = _day,
        .dotw = (signed char)rtc::day_of_week(_day, _month, _year), // 0 is Sunday, so 5 is Friday
        .hour = t.hour,
        .min = t.min,
        .sec = t.sec
        };

    rtc_set_datetime(&t);
    sleep_us(64);
}


void rtc_rp2040::init_rtc()
{
    rtc_init();
// IF WE USE THE INTERNAL RTC WE HAVE TO SET A TIME OTHERWISE THE CLOCK DOES NOT START
#ifdef SET_INITIAL_TIME_ZERO
    rtc_rp2040::set_rtc_time(0, 0, 0);
    rtc_rp2040::set_rtc_date(1, 1, 22);
#else
    rtc_rp2040::set_rtc_time(__TIME__);
    rtc_rp2040::set_rtc_date(__DATE__);
#endif
}

datetime_t rtc_rp2040::read_rtc()
{
    datetime_t t;
    rtc_get_datetime(&t);

    if(rtc::enable_daylightsaving && rtc::summertime_eu(t.year, t.month, t.day, t.hour)){
        t.hour = (t.hour + 1) % 24;
    } 
    
    return t;
}
