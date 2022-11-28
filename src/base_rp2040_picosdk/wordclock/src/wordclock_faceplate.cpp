#include "wordclock_faceplate.hpp"


wordclock_faceplate::wordclock_faceplate(){

}

wordclock_faceplate::~wordclock_faceplate(){

}


void wordclock_faceplate::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s){
    const std::vector<std::tuple<int, int>> corners = {{0, 0}, {LED_MATRIX_WIDTH-1, 0}, {0, LED_MATRIX_HEIGHT-1}, {LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT}};
    set_leds(_leds,corners, wordclock_faceplate::WORD_COLOR_CLASS::HOUR ,_s);
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




void wordclock_faceplate::set_leds(PicoLed::PicoLedController &_leds, const std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds)
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