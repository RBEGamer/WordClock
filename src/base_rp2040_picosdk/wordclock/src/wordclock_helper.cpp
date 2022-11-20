#include "wordclock_helper.h"

PicoLed::Color wordclock_helper::Wheel(int WheelPos, int _bright = 255)
{

    if (_bright > 255)
    {
        _bright = 255;
    }

    if (_bright < 0)
    {
        _bright = 0;
    }

    const float brgth_scale = _bright / 255.0;
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return PicoLed::RGB((255 - WheelPos * 3) * brgth_scale, 0, (WheelPos * 3) * brgth_scale);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return PicoLed::RGB(0, (WheelPos * 3) * brgth_scale, (255 - WheelPos * 3) * brgth_scale);
    }
    WheelPos -= 170;
    return PicoLed::RGB((WheelPos * 3) * brgth_scale, (255 - WheelPos * 3) * brgth_scale, 0);
}

PicoLed::Color wordclock_helper::get_word_color(const wordclock_helper::wordclock_word_index _word, const int _current_seconds)
{

    // calc position on color wheel
    const int wheel_pos = (255 / 60) * (_current_seconds % 60);
    const int offset = 255 / 4;
    // check for wordtype and add additional color offset to differ m h s in color
    if (_word == C_ES or _word == C_IST or _word == C_UHR)
    {
        return PicoLed::HSV(((wheel_pos + offset * 0) % 360), 255, 255);
        // MINUTES
    }
    else if (_word >= 2 && _word <= 8)
    {
        return PicoLed::HSV(((wheel_pos + offset * 1) % 360), 255, 255);
        // HOURS
    }
    else if (_word > 8 && _word <= 22)
    {
        return PicoLed::HSV(((wheel_pos + offset * 2) % 360), 255, 255);
        // MINUTE DOTS
    }
    else if (_word > 22 && _word <= 26)
    {
        return PicoLed::HSV(((wheel_pos + offset * 1) % 360), 255, 255);
    }
    else
    {
        return PicoLed::RGB(128, 128, 128);
    }
}

// 23 different word and max 10 leds per word (can be modified, see wordclock_word_index for wordindex)
int wordclock_helper::CLOCKWORDS[27][10] = {
    {1, 2, -1, -1, -1, -1, -1, -1, -1, -1},       // 0 es 0 X OK
    {4, 5, 6, -1, -1, -1, -1, -1, -1, -1},        // 1 ist 1 X OK
    {-1, 8, 9, 10, 11, -1, -1, -1, -1, -1},       // 2 fuenf 2 X OK
    {23, 22, 21, 20, -1, -1, -1, -1, -1, -1},     // 3 zehn 3 X OK
    {13, 14, 15, 16, 17, 18, 19, -1, -1, -1},     // 4  zwanzig 4 X OK
    {28, 29, 30, 31, 32, 33, 34, -1, -1, -1},     // 5 viertel 5 X OK
    {45, 44, 43, -1, -1, -1, -1, -1, -1, -1},     // 6 vor 6 X OK
    {35, 36, 37, 38, -1, -1, -1, -1, -1, -1},     // 7 nach 8 X OK
    {46, 47, 48, 49, -1, -1, -1, -1, -1, -1},     // 8 halb 7 X OK
    {106, 107, 108, 109, -1, -1, -1, -1, -1, -1}, // 9 neun 9  X OK
    {53, 54, 55, 56, -1, -1, -1, -1, -1, -1},     // 10 fuenf 10 X OK
    {67, 66, 65, 64, -1, -1, -1, -1, -1, -1},     // 11 eins 11 X OK
    {57, 58, 59, 60, -1, -1, -1, -1, -1, -1},     // 12 zwei 12 X OK
    {68, 69, 70, 71, -1, -1, -1, -1, -1, -1},     // 13 drei 13 X OK
    {78, 77, 76, 75, -1, -1, -1, -1, -1, -1},     // 14 vier 14 X OK
    {89, 88, 87, 86, 85, -1, -1, -1, -1, -1},     // 15 sechs 15 X OK
    {79, 80, 81, 82, -1, -1, -1, -1, -1, -1},     // 16 acht 16 X OK
    {90, 91, 92, 93, 94, 95, -1, -1, -1, -1},     // 17 sieben 17 X OK
    {96, 97, 98, 99, 100, -1, -1, -1, -1, -1},    // 18 zwölf 18 X OK
    {112, 111, 110, 109, -1, -1, -1, -1, -1, -1}, // 19 zehn 19 X OK
    {51, 52, 53, -1, -1, -1, -1, -1, -1, -1},     // 20 elf 20 X OK
    {102, 103, 104, -1, -1, -1, -1, -1, -1, -1},  // 21 uhr 21 X X
    {67, 66, 65, -1, -1, -1, -1, -1, -1, -1},     // 22 eins 11 X OK

    {0, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 23 minute dot one
    {12, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 24 minute dot two
    {101, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 25 minute dot three
    {113, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 26 minute dot four
};

void wordclock_helper::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{

    // MINUTE DOTS
    if (_m < 5)
    {
        const int minute_dots = _m % 5;
        if (minute_dots > 0)
        {
            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::MD_ONE, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
        if (minute_dots > 1)
        {

            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::MD_TWO, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
        if (minute_dots > 2)
        {

            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::MD_THREE, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
        if (minute_dots > 3)
        {
            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::MD_FOUR, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
    }

    // PREFIX WORDS IT IS xxx (OCLOCK)
    wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::C_ES, _s, PICO_DEFAULT_WS2812_OFFSET);
    wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::C_IST, _s, PICO_DEFAULT_WS2812_OFFSET);
    //after >30 min the hour word need to be set to the nex full hour
    int _h = _horig;
    if(_m > 30){
        _h = (_horig+1) % 24;
    }
    // HOURS
    if (_h == 0 || _h == 12)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_ZWOELF, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 1 || _h == 13)
    {
        if (_m >= 0 && _m < 5)
        {
            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_EIN, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
        else
        {
            wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_EINS, _s, PICO_DEFAULT_WS2812_OFFSET);
        }
    }
    else if (_h == 2 || _h == 14)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_ZWEI, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 3 || _h == 15)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_DREI, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 4 || _h == 16)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_VIER, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 5 || _h == 17)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_FUENF, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 6 || _h == 18)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_SECHS, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 7 || _h == 19)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_SIEBEN, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 8 || _h == 20)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_ACHT, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 9 || _h == 21)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_NEUN, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 10 || _h == 22)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_ZEHN, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_h == 11 || _h == 23)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::H_ELF, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    // ADD MIDDLE WORDS
    if (_m >= 5 && _m < 25)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_NACH, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 35 && _m < 40)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_NACH, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 25 && _m < 30)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_VOR, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 40 && _m < 59)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_VOR, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 0 && _m < 5)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::C_UHR, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    // ADD WORDS FOR MINUTES
    if (_m >= 5 && _m < 10)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_FUENF, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 10 && _m < 15)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_ZEHN, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 15 && _m < 20)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_VIERTEL, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 20 && _m < 25)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_ZWANZIG, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 25 && _m < 30)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_FUENF, _s, PICO_DEFAULT_WS2812_OFFSET);
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_HALB, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 30 && _m < 35)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_HALB, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 35 && _m < 40)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_FUENF, _s, PICO_DEFAULT_WS2812_OFFSET);
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_HALB, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 40 && _m < 45)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_ZWANZIG, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 45 && _m < 50)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_VIERTEL, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 50 && _m < 55)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_ZEHN, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
    else if (_m >= 55 && _m < 60)
    {
        wordclock_helper::set_word(_leds, wordclock_helper::wordclock_word_index::M_FUENF, _s, PICO_DEFAULT_WS2812_OFFSET);
    }
}

void wordclock_helper::display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    // PICO_DEFAULT_WS2812_OFFSET
    _leds.clear();
    display_time_with_words(_leds, _h, _m, _s);
    _leds.show();
}

void wordclock_helper::set_word(PicoLed::PicoLedController &_leds, const wordclock_helper::wordclock_word_index _word, const int _current_seconds, const int led_index_offset)
{
    const int word_index = (int)_word;
    const int word_len = sizeof(wordclock_helper::CLOCKWORDS[word_index]) / sizeof(wordclock_helper::CLOCKWORDS[word_index][0]);
    

    for (int i = 0; i < word_len; i++)
    {
        if (wordclock_helper::CLOCKWORDS[word_index][i] >= 0)
        {
            _leds.setPixelColor(wordclock_helper::CLOCKWORDS[word_index][i] + led_index_offset, get_word_color(_word, _current_seconds));
        }
    }
}