#ifndef __AMBIENT_LIGHT_H__
#define __AMBIENT_LIGHT_H__

#include "pico/stdlib.h"


#include "../helper.h"
class ambient_light
{
public:

    ambient_light();
    virtual ~ambient_light();

    
    virtual int get_brightness();
    virtual int get_average_brightness();
    virtual void init();


};
#endif