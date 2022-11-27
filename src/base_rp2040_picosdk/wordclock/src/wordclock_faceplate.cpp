#include "wordclock_faceplate.hpp"


wordclock_faceplate::wordclock_faceplate(){

}



PicoLed::Color wordclock_faceplate::get_word_color_by_class(const wordclock_faceplate::WORD_COLOR_CLASS _basecolor, const int _current_seconds){
    // calc position on color wheel
    const int wheel_pos = (255 / 60) * (_current_seconds % 60);
    const int offset = 255 / (int)wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT;

    // COLOR DEPENDING WORD TYPE
    if (_basecolor == wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT)
    {
       return PicoLed::RGB(128, 128, 128);
    }
    else
    {
        return PicoLed::HSV(((wheel_pos + offset * (int)_basecolor)), 255, 255);
    }
}




void wordclock_faceplate::set_word(PicoLed::PicoLedController &_leds, const std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds)
{
   

    for (int i = 0; i < _word.size(); i++)
    {
        const std::tuple<int, int> led_pos = _word.at(i);
        const int x = std::get<0>(led_pos);
        const int y = std::get<1>(led_pos);


        if (x == USE_DIRECT_LED_INDEXING){
            _leds.setPixelColor(y, get_word_color_by_class(_basecolor, _current_seconds));
        }else{
            //TODO INTRODUCE MATRIX
        }
        
    }
}


void wordclock_faceplate::display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    // PICO_DEFAULT_WS2812_OFFSET
    _leds.clear();
    // TODO IMPLEMENT TRANSITION
    display_time_with_words(_leds, _h, _m, _s);
    _leds.show();
}