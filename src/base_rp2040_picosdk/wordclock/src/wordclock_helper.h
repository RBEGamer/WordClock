#ifndef __WORDCLOCK_HELPER_H__
#define __WORDCLOCK_HELPER_H__

#include <PicoLed.hpp>

class wordclock_helper
{
public:
  // CLOCKWORDS INDEX FOR CLOCKWORDS ÖED INDE ARRAY
  enum wordclock_word_index
  {
    C_ES = 0,
    C_IST = 1,
    C_UHR = 21,
    M_FUENF = 2,
    M_ZEHN = 3,
    M_ZWANZIG = 4,
    M_VIERTEL = 5,
    M_VOR = 6,
    M_NACH = 7,
    M_HALB = 8,
    H_EINS = 11,
    H_ZWEI = 12,
    H_DREI = 13,
    H_VIER = 14,
    H_FUENF = 10,
    H_SECHS = 15,
    H_SIEBEN = 17,
    H_ACHT = 16,
    H_NEUN = 9,
    H_ZEHN = 19,
    H_ELF = 20,
    H_ZWOELF = 18,
    H_EIN = 22,
    MD_ONE = 23,
    MD_TWO = 24,
    MD_THREE = 25,
    MD_FOUR = 26
  };

  // 27 different word and max 10 leds per word (can be modified, see wordclock_word_index for wordindex) see wordclock_helper.cpp
  static int CLOCKWORDS[27][10];

  static void display_time_with_words(PicoLed::PicoLedController& _leds, const int _horig, const int _m, const int _s);
  static void display_time(PicoLed::PicoLedController& _leds, const int _h, const int _m, const int _s);
  static void set_word(PicoLed::PicoLedController& _leds, const wordclock_helper::wordclock_word_index _word, const int _current_seconds, const int led_index_offset);
  static PicoLed::Color Wheel(int WheelPos, int _bright);
  static PicoLed::Color get_word_color(const wordclock_word_index _word, const int _current_seconds);
};

#endif