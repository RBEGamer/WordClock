#ifndef __WORDCLOCK_FACEPLATE_H__
#define __WORDCLOCK_FACEPLATE_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>
#include "wordclock_faceplate_constants.h"
class wordclock_faceplate
{
protected:
  enum class WORD_COLOR_CLASS
  {
    HOUR,
    MINUTE,
    COMMON,
    DEFAULT // DONT DELETE PLACE AS lAST ELEMENT
  };

  // MAKE PURE VIRTUAL = INTERFACE SO NO IMPLEMENTATION IN BASE NEEDED
  virtual void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s);
  void set_leds(PicoLed::PicoLedController &_leds, std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds);
  PicoLed::Color get_word_color_by_class(const WORD_COLOR_CLASS _basecolor, const int _current_seconds);


  int xy_to_led_index(const int _x, const int _y);
  
public:

 



  wordclock_faceplate();
  virtual ~wordclock_faceplate();
  void display_testpattern();
  void display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s);

  
};

#endif
