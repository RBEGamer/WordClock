#ifndef __WORDCLOCK_FACEPLATE_DOTS_H__
#define __WORDCLOCK_FACEPLATE_DOTS_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "helper.h"
#include "wordclock_faceplate.hpp"

class wordclock_faceplate_dots : public wordclock_faceplate
{


public:
    wordclock_faceplate_dots();
    ~wordclock_faceplate_dots();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;
};
#endif
