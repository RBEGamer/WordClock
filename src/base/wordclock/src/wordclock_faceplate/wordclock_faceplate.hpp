#ifndef __WORDCLOCK_FACEPLATE_H__
#define __WORDCLOCK_FACEPLATE_H__
#include <pico/platform.h>
#include <PicoLed.hpp>
#include <vector>
#include <tuple>
#include "wordclock_faceplate_constants.h"
#include "../wifi_interface.h"

class wordclock_faceplate
{
protected:
  enum class WORD_COLOR_CLASS
  {
    HOUR,
    MINUTE,
    COMMON,
    DEFAULT // DONT DELETE PLACE AS lAST ELEMENT used as length indicator
  };

  virtual void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s);
  void set_leds(PicoLed::PicoLedController &_leds, std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds);
  PicoLed::Color get_word_color_by_class(const WORD_COLOR_CLASS _basecolor, const int _current_seconds);
  int xy_to_led_index(const std::tuple<int, int> _xy);
  std::tuple<int, int> flip_xy(const std::tuple<int, int> _origin, const bool _flip);

public:
  enum class FACEPLATES
  {
    GERMAN,
    ENGLISH,
    BINARY,
    DOTS,
    TEST // DONT DELETE TEST PLACE AS lAST ELEMENT used as length indicator
  };

  enum class COLORMODE
  {
    RAINBOW_SEPARATE = 0,
    RAINBOW_SAME = 1,
    COLD_WHITE = 2,
    WARM_WHITE = 3,
    LENGTH //DONT DELETE or USE
  }; 
  
  struct FACEPLATE_CONFIG
  {
    bool flip_state;
    FACEPLATES current_faceplate;
    COLORMODE color_mode;
    FACEPLATE_CONFIG()
    {
      flip_state = WORDCLOCK_DISPlAY_ORIENTATION;
      current_faceplate = FACEPLATES::TEST;
      color_mode = COLORMODE::RAINBOW_SEPARATE;
    }
  };

  // INLINE IN C++17 no init in cpp needed
  inline static struct FACEPLATE_CONFIG config;

  wordclock_faceplate();
  virtual ~wordclock_faceplate();

  void display_testpattern(PicoLed::PicoLedController &_leds);
  void display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s);
  void set_colormode(COLORMODE _colormode);

};

#endif
