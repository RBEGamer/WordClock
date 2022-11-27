#include "wordclock_faceplate.hpp"



PicoLed::Color wordclock_faceplate::get_word_color_by_class(const wordclock_faceplate::WORD_COLOR_CLASS _word, const int _current_seconds);

{
    // calc position on color wheel
    const int wheel_pos = (255 / 60) * (_current_seconds % 60);
    const int offset = 255 / 4;

    // COLOR DEPENDING WORD TYPE
    if (_word == wordclock_faceplate::WORD_COLOR_CLASS::MINUTE)
    {
        return PicoLed::HSV(((wheel_pos + offset * 0)), 255, 255);
    }
    else if (_word == wordclock_faceplate::WORD_COLOR_CLASS::HOUR)
    {
        return PicoLed::HSV(((wheel_pos + offset * 1)), 255, 255);
    }
    else if (_word == wordclock_faceplate::WORD_COLOR_CLASS::SECOND)
    {
        return PicoLed::HSV(((wheel_pos + offset * 2)), 255, 255);
    }
    else
    {
        return PicoLed::RGB(128, 128, 128);
    }
}




void wordclock_faceplate::set_word(PicoLed::PicoLedController &_leds, const std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds)
{
    const int word_index = (int)_word;

    for (int i = 0; i < _word.size(); i++)
    {
        const std::tuple<int, int> led_pos = _word.at(i);
        const int x = std::get<0>(led_pos);
        const int y = std::get<1>(led_pos);
        const wordclock_faceplate::WORD_COLOR_CASS color =  std::get<2>(led_pos);

        if (x == USE_DIRECT_LED_INDEXING){
            _leds.setPixelColor(y, get_word_color_by_class(color, _current_seconds));
        }else{
            //TODO INTRODUCE MATRIX
        }
        
    }
}


void wordclock_helper::display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    // PICO_DEFAULT_WS2812_OFFSET
    _leds.clear();
    // TODO IMPLEMENT TRANSITION
    display_time_with_words(_leds, _h, _m, _s);
    _leds.show();
}