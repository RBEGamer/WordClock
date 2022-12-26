#include "ambient_light.hpp"

int ambient_light::brightness_curve = WORDCLOCK_BRIGHTNESS_MODE_AUTO_CURVE;

ambient_light::ambient_light()
{
}
ambient_light::~ambient_light()
{
}

int ambient_light::get_brightness()
{
    return (WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN + WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX) / 2;
}

int ambient_light::get_average_brightness()
{
    return ambient_light::get_brightness();
}

void ambient_light::init()
{
}

void ambient_light::set_brightness_curve(const int _brightness_curve)
{
    if (_brightness_curve < 10)
    {
        ambient_light::brightness_curve = 10;
    }
    ambient_light::brightness_curve = helper::limit(abs(_brightness_curve), 10, 100);
}
