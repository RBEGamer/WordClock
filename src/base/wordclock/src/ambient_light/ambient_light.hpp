#ifndef __AMBIENT_LIGHT_H__
#define __AMBIENT_LIGHT_H__

#include "pico/stdlib.h"


#include "../helper.h"
class ambient_light
{

protected:
   static inline int brightness_curve = WORDCLOCK_BRIGHTNESS_MODE_AUTO_CURVE;

public:


    ambient_light();
    virtual ~ambient_light();
    virtual int get_brightness();
    virtual int get_average_brightness();
    virtual void init();

    void set_brightness_curve(const int _brightness_curve);




};
#endif