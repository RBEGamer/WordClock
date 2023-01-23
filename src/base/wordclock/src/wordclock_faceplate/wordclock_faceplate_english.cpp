#include "wordclock_faceplate_english.hpp"

wordclock_faceplate_english::wordclock_faceplate_english()
{
}

wordclock_faceplate_english::~wordclock_faceplate_english()
{
}

// SET WORD FUNCTION ALSO COLORS THE DIFFERENT WORD TYPES
// AND USES THE SET LED FUNCTION FROM BASECLASS
void wordclock_faceplate_english::set_word(PicoLed::PicoLedController &_leds, const wordclock_faceplate_english::WORDS_INDEX _word, const int _current_seconds)
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

void wordclock_faceplate_english::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{
    // MINUTE DOTS
    const int minute_dots = _m % 5;
    if (minute_dots > 0 || (BLINKENDOTS == 1 && _s%2 == 0))
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_DOT_ONE, _s);
    }

    if (minute_dots > 1 || (BLINKENDOTS == 1 && _s%2 == 0))
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_DOT_TWO, _s);
    }

    if (minute_dots > 2 || (BLINKENDOTS == 1 && _s%2 == 0))
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_DOT_THREE, _s);
    }
    
    if (minute_dots > 3 || (BLINKENDOTS == 1 && _s%2 == 0))
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_DOT_FOUR, _s);
    }

    // PREFIX WORDS IT IS xxx (OCLOCK)
    wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::C_IT, _s);
    wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::C_IS, _s);

    if (_horig > 12)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_PM, _s);
    }
    else
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_AM, _s);
    }

    // after >30 min the hour word need to be set to the nex full hour
    int _h = _horig;
    if (_m >= 40)
    {
        _h = (_horig + 1) % 24;
    }
    // HOURS
    if (_h == 0 || _h == 12)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_TWELVE, _s);
    }
    else if (_h == 1 || _h == 13)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_ONE, _s);
    }
    else if (_h == 2 || _h == 14)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_TWO, _s);
    }
    else if (_h == 3 || _h == 15)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_THREE, _s);
    }
    else if (_h == 4 || _h == 16)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_FOUR, _s);
    }
    else if (_h == 5 || _h == 17)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_FIVE, _s);
    }
    else if (_h == 6 || _h == 18)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_SIX, _s);
    }
    else if (_h == 7 || _h == 19)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_SEVEN, _s);
    }
    else if (_h == 8 || _h == 20)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_EIGHT, _s);
    }
    else if (_h == 9 || _h == 21)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_NINE, _s);
    }
    else if (_h == 10 || _h == 22)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_TEN, _s);
    }
    else if (_h == 11 || _h == 23)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::H_ELEVEN, _s);
    }
    // ADD MIDDLE WORDS
    if (_m >= 5 && _m < 25)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_PAST, _s);
    }
    else if (_m >= 30 && _m < 40)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_PAST, _s);
    }
    else if (_m >= 25 && _m < 30)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TO, _s);
    }
    else if (_m >= 40 && _m <= 59)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TO, _s);
    }
    else if (_m >= 0 && _m < 5)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::C_OCLOCK, _s);
    }
    // ADD WORDS FOR MINUTES
    if (_m >= 5 && _m < 10)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_FIVE, _s);
    }
    else if (_m >= 10 && _m < 15)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TEN, _s);
    }
    else if (_m >= 15 && _m < 20)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_QUATER, _s);
    }
    else if (_m >= 20 && _m < 25)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TWENTY, _s);
    }
    else if (_m >= 25 && _m < 30)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_FIVE, _s);
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_HALF, _s);
    }
    else if (_m >= 30 && _m < 35)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_HALF, _s);
    }
    else if (_m >= 35 && _m < 40)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_FIVE, _s);
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_HALF, _s);
    }
    else if (_m >= 40 && _m < 45)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TWENTY, _s);
    }
    else if (_m >= 45 && _m < 50)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_QUATER, _s);
    }
    else if (_m >= 50 && _m < 55)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_TEN, _s);
    }
    else if (_m >= 55 && _m < 60)
    {
        wordclock_faceplate_english::set_word(_leds, wordclock_faceplate_english::WORDS_INDEX::M_FIVE, _s);
    }
}
