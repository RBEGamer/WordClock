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
        wordclock_faceplate_dots::set_word(_leds, wordclock_faceplate_dots::WORDS_INDEX::S_SECONDS_1, _s);
    }

    if (_s > 25)
    {
        wordclock_faceplate_dots::set_word(_leds, wordclock_faceplate_dots::WORDS_INDEX::S_SECONDS_2, _s);
    }
    if (_s > 40)
    {
        wordclock_faceplate_dots::set_word(_leds, wordclock_faceplate_dots::WORDS_INDEX::S_SECONDS_3, _s);
    }
    if (_s > 55)
    {
        wordclock_faceplate_dots::set_word(_leds, wordclock_faceplate_dots::WORDS_INDEX::S_SECONDS_4, _s);
    }



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
    for(size_t m = 0; m < _m; h++){
        wordclock_faceplate::set_leds(_leds, {{LED_MATRIX_HEIGHT - 1 - curr_x, curr_y}}, wordclock_faceplate::WORD_COLOR_CLASS::MINUTE, _s);
        curr_y++;
        if(curr_y >= LED_MATRIX_WIDTH){
            curr_y = 0;
            curr_x++;
        }
    }

}
