#ifndef __AMBIENT_LIGHT_BH1750_H__
#define __AMBIENT_LIGHT_BH1750_H__

#include "pico/stdlib.h"

#include "../helper.h"
#include "ambient_light.h"


class ambient_light_bh1750: public ambient_light
{
public:

    ambient_light_bh1750();
    ~ambient_light_bh1750();
    
    virtual int get_brightness() override;
    virtual int get_average_brightness() override;
    virtual void init() override;

protected:
    // ROLLING AVERAGE
    static const int NUM_READINGS = 30;
    static inline int readings[NUM_READINGS] = { 0 };

    int reading_index = 0;
    long avg_sum = 0;

};
#endif