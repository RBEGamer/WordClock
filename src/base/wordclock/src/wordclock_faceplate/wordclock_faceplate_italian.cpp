#include "wordclock_faceplate_italian.hpp"

wordclock_faceplate_italian::wordclock_faceplate_italian()
{
}

wordclock_faceplate_italian::~wordclock_faceplate_italian()
{
}

// SET WORD FUNCTION ALSO COLORS THE DIFFERENT WORD TYPES
// AND USES THE SET LED FUNCTION FROM BASECLASS
void wordclock_faceplate_italian::set_word(PicoLed::PicoLedController &_leds, const wordclock_faceplate_italian::WORDS_INDEX _word, const int _current_seconds)
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

void wordclock_faceplate_italian::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{
    // MINUTE DOTS
    const int minute_dots = _m % 5;
    if (minute_dots > 0 || (BLINKENDOTS == 1 && _s % 2 == 0))
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DOT_ONE, _s);
    }

    if (minute_dots > 1 || (BLINKENDOTS == 1 && _s % 2 == 0))
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DOT_TWO, _s);
    }
    if (minute_dots > 2 || (BLINKENDOTS == 1 && _s % 2 == 0))
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DOT_THREE, _s);
    }
    if (minute_dots > 3 || (BLINKENDOTS == 1 && _s % 2 == 0))
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DOT_FOUR, _s);
    }

    // PREFIX WORDS SONE LE ORE
    wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::C_SONO, _s);
    wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::C_LE, _s);
    wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::C_ORE, _s);
    // after >30 min the hour word need to be set to the nex full hour
    //int _h = _horig;
    //if (_m >= 25)
    //{
    //    _h = (_horig + 1) % 24;
    //}
    // HOURS
    if (_h == 0 || _h == 12)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_DODICI, _s);
    }
    else if (_h == 1 || _h == 13)
    {

        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_UNA, _s);
    }
    else if (_h == 2 || _h == 14)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_DUE, _s);
    }
    else if (_h == 3 || _h == 15)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_TRE, _s);
    }
    else if (_h == 4 || _h == 16)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_QUATTRO, _s);
    }
    else if (_h == 5 || _h == 17)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_CINQUE, _s);
    }
    else if (_h == 6 || _h == 18)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_SEI, _s);
    }
    else if (_h == 7 || _h == 19)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_SETTE, _s);
    }
    else if (_h == 8 || _h == 20)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_OTTO, _s);
    }
    else if (_h == 9 || _h == 21)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_NOVE, _s);
    }
    else if (_h == 10 || _h == 22)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_DIECI, _s);
    }
    else if (_h == 11 || _h == 23)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::H_UNDICI, _s);
    }

    // ADD MIDDLE WORDS TO PAST VOR NACH
    if (_m >= 5 && _m < 35)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_E, _s);
    }
    else if (_m >= 35 && _m <= 59)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_MENO, _s);
    }


    // ADD WORDS FOR MINUTES
    if (_m >= 5 && _m < 10)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_CINIQUE, _s);
    }
    else if (_m >= 10 && _m < 15)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DIECI, _s);
    }
    else if (_m >= 15 && _m < 20)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_UN, _s);
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_QUARTO, _s);
    }
    else if (_m >= 20 && _m < 25)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_VENTI, _s);
    }
    else if (_m >= 25 && _m < 30)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_MEZZA, _s);

    }
    else if (_m >= 30 && _m < 35)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_MEZZA, _s);
    }
    else if (_m >= 35 && _m < 40)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_CINIQUE, _s);
    }
    else if (_m >= 40 && _m < 45)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_VENTI, _s);
    }
    else if (_m >= 45 && _m < 50)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_UN, _s);
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_QUARTO, _s);
    }
    else if (_m >= 50 && _m < 55)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_DIECI, _s);
    }
    else if (_m >= 55 && _m < 60)
    {
        wordclock_faceplate_italian::set_word(_leds, wordclock_faceplate_italian::WORDS_INDEX::M_CINIQUE, _s);
    }
}
