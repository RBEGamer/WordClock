#include "wordclock_faceplate_template.hpp"

wordclock_faceplate_template::wordclock_faceplate_template()
{
}

wordclock_faceplate_template::~wordclock_faceplate_template()
{
}

// SET WORD FUNCTION ALSO COLORS THE DIFFERENT WORD TYPES
// AND USES THE SET LED FUNCTION FROM BASECLASS
// YOU CAN IMPLEMENT YOUR OWN COLORING HERE OR IN display_time_with_words AND DELETE THIS FUNCTION
void wordclock_faceplate_template::set_word(PicoLed::PicoLedController &_leds, const wordclock_faceplate_template::WORDS_INDEX _word, const int _current_seconds)
{
    wordclock_faceplate::WORD_COLOR_CLASS color = wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT;
    if (_word > WORDS_INDEX::WORDS_INDEX_COMMON_BEGIN && _word < WORDS_INDEX::WORDS_INDEX_COMMON_END)
    {
        color = wordclock_faceplate::WORD_COLOR_CLASS::COMMON;
    }
    else if (_word > WORDS_INDEX::WORDS_INDEX_MINUTE_BEGIN && _word < WORDS_INDEX::WORDS_INDEX_MINUTE_END)
    {
        color = wordclock_faceplate::WORD_COLOR_CLASS::MINUTE;
    }
    else if (_word > WORDS_INDEX::WORDS_INDEX_HOUR_BEGIN && _word < WORDS_INDEX::WORDS_INDEX_HOUR_END)
    {
        color = wordclock_faceplate::WORD_COLOR_CLASS::HOUR;
    }

    wordclock_faceplate::set_leds(_leds, WORDCLOCKwORDS[(int)_word], color, _current_seconds);
}


void wordclock_faceplate_template::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{
    

   
    // PREFIX WORDS IT IS xxx (OCLOCK)
    wordclock_faceplate_template::set_word(_leds, wordclock_faceplate_template::WORDS_INDEX::C_ES, _s);

   
    // HOURS
    if (_h == 1 || _h == 13)
    {
        wordclock_faceplate_template::set_word(_leds, wordclock_faceplate_template::WORDS_INDEX::H_EINS, _s);
    }

    //MINUTES
   if (_m % 5 == 0)
    {
        wordclock_faceplate_template::set_word(_leds, wordclock_faceplate_template::WORDS_INDEX::M_FUENF, _s);
    }
}
