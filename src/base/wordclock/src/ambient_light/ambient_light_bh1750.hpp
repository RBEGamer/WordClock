#ifndef __AMBIENT_LIGHT_BH1750_H__
#define __AMBIENT_LIGHT_BH1750_H__

#include "pico/stdlib.h"

#include "../helper.h"
#include "ambient_light.hpp"

//DISABLE READING AVERAGE BY SETTING TO 1
#define AMBIENT_LIGHT_BH1750_NUM_READINGS 1
class ambient_light_bh1750: public ambient_light
{

protected:
    // ROLLING AVERAGE
    
#if AMBIENT_LIGHT_BH1750_NUM_READINGS > 1
    static inline int readings[AMBIENT_LIGHT_BH1750_NUM_READINGS] = { 0 };
    int reading_index = 0;
    long avg_sum = 0;
    long average = 0;
#endif
    // these lux values will be mapped from 0-50 to led WORDCLOC_BRIGHTNESS_MODE_AUTO_MIN-WORDCLOC_BRIGHTNESS_MODE_AUTO_MAX values
    const int BH1750_RAW_OUTPUT_MIN = 0;
    const int BH1750_RAW_OUTPUT_MAX = 150;

    
public:

    ambient_light_bh1750();
    ~ambient_light_bh1750();
    
    int get_brightness() override;
    int get_average_brightness() override;
    void init() override;




};
#endif