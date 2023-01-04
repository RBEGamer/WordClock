#include "wordclock_faceplate.hpp"

wordclock_faceplate::wordclock_faceplate()
{
}

wordclock_faceplate::~wordclock_faceplate()
{
}

void wordclock_faceplate::display_testpattern(PicoLed::PicoLedController &_leds)
{
    wordclock_faceplate::display_time(_leds, 0, 0, 0);
}

void wordclock_faceplate::display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s)
{
    const std::vector<std::tuple<int, int>> corners = {{LED_MATRIX_HEIGHT - 1, LED_MATRIX_WIDTH - 1}, {0, LED_MATRIX_WIDTH - 1}, {LED_MATRIX_HEIGHT - 1, 0}, {0, 0}};
    set_leds(_leds, corners, wordclock_faceplate::WORD_COLOR_CLASS::HOUR, _s);
    const std::vector<std::tuple<int, int>> test = {{USE_DIRECT_LED_INDEXING, 0}, {USE_DIRECT_LED_INDEXING, 12}, {USE_DIRECT_LED_INDEXING, 101}, {USE_DIRECT_LED_INDEXING, 113}};
    set_leds(_leds, test, wordclock_faceplate::WORD_COLOR_CLASS::MINUTE, _s);
}

PicoLed::Color wordclock_faceplate::get_word_color_by_class(const wordclock_faceplate::WORD_COLOR_CLASS _basecolor, const int _current_seconds)
{
    // return PicoLed::RGB(128, 128, 128);

    if (_basecolor == wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT)
    {
        return PicoLed::RGB(128, 128, 128);
    }

    // calc position on color wheel
    const int wheel_pos = (255 / 60) * (_current_seconds % 60);

    if (wordclock_faceplate::config.color_mode == wordclock_faceplate::COLORMODE::RAINBOW_SEPARATE)
    {
        const int offset = 255 / (int)wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT;
        return PicoLed::HSV(((wheel_pos + offset * (int)_basecolor)), 255, 255);
    }
    else if (wordclock_faceplate::config.color_mode == wordclock_faceplate::COLORMODE::RAINBOW_SAME)
    {
        return PicoLed::HSV(wheel_pos, 255, 255);
    }
    else if (wordclock_faceplate::config.color_mode == wordclock_faceplate::COLORMODE::COLD_WHITE)
    {
        return PicoLed::RGB(255, 250, 250);
    }
    else if (wordclock_faceplate::config.color_mode == wordclock_faceplate::COLORMODE::WARM_WHITE)
    {
        return PicoLed::RGB(184, 115, 51);
    }
    else
    {
        return PicoLed::RGB(128, 128, 128);
    }
}

std::tuple<int, int> wordclock_faceplate::flip_xy(const std::tuple<int, int> _origin, const bool _flip)
{

    if (_flip)
    {
        return {LED_MATRIX_HEIGHT - 1 - std::get<0>(_origin), LED_MATRIX_WIDTH - 1 - std::get<1>(_origin)};
    }

    return _origin;
}

int wordclock_faceplate::xy_to_led_index(const std::tuple<int, int> _xy)
{
    // TODO ROTATE POINTS
    const int x = std::get<1>(_xy);
    const int y = std::get<0>(_xy);
    int index = 0;

    if (x >= LED_MATRIX_WIDTH || y >= LED_MATRIX_HEIGHT)
    {
        wifi_interface::send_log("xy_to_led_index out of range");
        return 0;
    }

    if ((y % 2) == 0)
    {
        index = x + (y * LED_MATRIX_WIDTH);
    }
    else
    {
        index = (LED_MATRIX_WIDTH - x) + (y * LED_MATRIX_WIDTH) - 1;
    }

    // ADD MINUTE DOT OFFSET
    int minute_dot_offset = 0;
    int minute_dots_passed = 0;
    for (size_t i = 0; i < LED_MINUTEDOT_COUNT; i++)
    {
        minute_dots_passed++;

        if ((index + minute_dots_passed) > LED_MINUTEDOT_POSITIONS[i])
        {
            minute_dot_offset++;
        }
    }
    return index + minute_dot_offset;
}

void wordclock_faceplate::set_leds(PicoLed::PicoLedController &_leds, const std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds)
{

    for (int i = 0; i < _word.size(); i++)
    {
        const std::tuple<int, int> led_pos = _word.at(i);

        if (std::get<0>(led_pos) == USE_DIRECT_LED_INDEXING)
        {
            const int clamped_index = std::get<1>(led_pos) % PICO_DEFAULT_WS2812_NUM;
            _leds.setPixelColor(clamped_index + LED_MATRIX_START_OFFSET, get_word_color_by_class(_basecolor, _current_seconds));
        }
        else
        {
            // FLIP PIXEL IF NEEDED
            const std::tuple<int, int> final = flip_xy(led_pos, wordclock_faceplate::config.flip_state);
            _leds.setPixelColor(xy_to_led_index(final) + LED_MATRIX_START_OFFSET, get_word_color_by_class(_basecolor, _current_seconds));
        }
    }
}

void wordclock_faceplate::display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    // PICO_DEFAULT_WS2812_OFFSET
    _leds.clear();
    // TODO IMPLEMENT TRANSITION
    display_time_with_words(_leds, _h, _m, _s);
    _leds.show();
}

void wordclock_faceplate::set_colormode(wordclock_faceplate::COLORMODE _colormode)
{
    if (_colormode < wordclock_faceplate::COLORMODE::RAINBOW_SEPARATE || _colormode >= wordclock_faceplate::COLORMODE::LENGTH)
    {
        return;
    }

    wordclock_faceplate::config.color_mode = _colormode;
}