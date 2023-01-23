#include "ambient_light.hpp"


ambient_light::ambient_light()
{
}

ambient_light::~ambient_light()
{
}

int ambient_light::get_brightness()
{
    //return ((WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN + WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX) / 2);
    return WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX;
}

int ambient_light::get_average_brightness()
{
    return ambient_light::get_brightness();
}

void ambient_light::init()
{
}
