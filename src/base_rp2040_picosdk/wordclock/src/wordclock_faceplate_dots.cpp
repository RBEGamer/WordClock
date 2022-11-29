#include "wordclock_faceplate_dots.hpp"

wordclock_faceplate_dots::wordclock_faceplate_dots()
{
}

wordclock_faceplate_dots::~wordclock_faceplate_dots()
{
}





void wordclock_faceplate_dots::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{
     
    if (_s > 10)
    {
       wordclock_faceplate::set_leds(_leds, {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[0]}}, wordclock_faceplate::WORD_COLOR_CLASS::COMMON, _s);
    }

    if (_s > 25)
    {
       wordclock_faceplate::set_leds(_leds, {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[1]}}, wordclock_faceplate::WORD_COLOR_CLASS::COMMON, _s);
    }
    if (_s > 40)
    {
         wordclock_faceplate::set_leds(_leds, {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[2]}}, wordclock_faceplate::WORD_COLOR_CLASS::COMMON, _s);
    }
    if (_s > 55)
    {
         wordclock_faceplate::set_leds(_leds, {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[3]}}, wordclock_faceplate::WORD_COLOR_CLASS::COMMON, _s);
    }


    // DIRECT LED ADRESSING
    int curr_x = 0;
    int curr_y = 0;
    for(size_t h = 0; h < _horig; h++){
        wordclock_faceplate::set_leds(_leds, {{curr_x, curr_y}}, wordclock_faceplate::WORD_COLOR_CLASS::HOUR, _s);
        curr_y++;
        if(curr_y >= LED_MATRIX_WIDTH){
            curr_y = 0;
            curr_x++;
        }
    }

    curr_x = 0;
    curr_y = 0;
    for(size_t m = 0; m < _m; m++){
        wordclock_faceplate::set_leds(_leds, {{LED_MATRIX_HEIGHT - 1 - curr_x, curr_y}}, wordclock_faceplate::WORD_COLOR_CLASS::MINUTE, _s);
        curr_y++;
        if(curr_y >= LED_MATRIX_WIDTH){
            curr_y = 0;
            curr_x++;
        }
    }

}
