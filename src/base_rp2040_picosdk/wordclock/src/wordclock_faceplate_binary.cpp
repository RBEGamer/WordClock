#include "wordclock_faceplate_binary.hpp"

wordclock_faceplate_binary::wordclock_faceplate_binary()
{
}

wordclock_faceplate_binary::~wordclock_faceplate_binary()
{
}

// SET WORD FUNCTION ALSO COLORS THE DIFFERENT WORD TYPES
// AND USES THE SET LED FUNCTION FROM BASECLASS
void wordclock_faceplate_binary::set_word(PicoLed::PicoLedController &_leds, const wordclock_faceplate_binary::WORDS_INDEX _word, const int _current_seconds)
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

void wordclock_faceplate_binary::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{

    const std::vector<bool> min_bits_ones = helper::bits_from_int(_m);
    const std::vector<bool> min_bits_tens = helper::bits_from_int(_m/10);
    const std::vector<bool> hour_bits_ones = helper::bits_from_int(_horig);
    const std::vector<bool> hour_bits_tens = helper::bits_from_int(_horig/10);

    if (min_bits_ones[0])
    {
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_ONES_ONE, _s);
    }
    if(min_bits_ones[1]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_ONES_TWO, _s);
    }
    if(min_bits_ones[2]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_ONES_FOUR, _s);
    }
    if(min_bits_ones[3]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_ONES_EIGHT, _s);
    }


    if (min_bits_tens[0])
    {
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_TENS_ONE, _s);
    }
    if(min_bits_tens[1]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_TENS_TWO, _s);
    }
    if(min_bits_tens[2]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_TENS_FOUR, _s);
    }
    if(min_bits_tens[3]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::M_TENS_EIGHT, _s);
    }


    if (hour_bits_ones[0])
    {
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_ONES_ONE, _s);
    }
    if(hour_bits_ones[1]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_ONES_TWO, _s);
    }
    if(hour_bits_ones[2]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_ONES_FOUR, _s);
    }
    if(hour_bits_ones[3]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_ONES_EIGHT, _s);
    }


    if (hour_bits_tens[0])
    {
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_TENS_ONE, _s);
    }
    if(hour_bits_tens[1]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_TENS_TWO, _s);
    }
    if(hour_bits_tens[2]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_TENS_FOUR, _s);
    }
    if(hour_bits_tens[3]){
        wordclock_faceplate_binary::set_word(_leds, wordclock_faceplate_binary::WORDS_INDEX::H_TENS_EIGHT, _s);
    }
}
