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

#ifdef RTCMODE
    const std::vector<std::tuple<int, int>> rtcmode = {{1, RTCMODE}};
    set_leds(_leds, rtcmode, wordclock_faceplate::WORD_COLOR_CLASS::MINUTE, _s);
#endif

#ifdef STORAGEMODE
    const std::vector<std::tuple<int, int>> storagemode = {{2, STORAGEMODE}};
    set_leds(_leds, storagemode, wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT, _s);
#endif

#ifdef LIGHTMODE
    const std::vector<std::tuple<int, int>> lightmode = {{3, LIGHTMODE}};
    set_leds(_leds, lightmode, wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT, _s);
#endif

#ifdef VERSION
    const char *delim = ".";
    std::vector<std::string> out;
    helper::tokenize(VERSION, delim, out);
    if (out.size() > 2)
    {
        const std::vector<std::tuple<int, int>> versionmdode = {{4, std::atoi(out.at(0).c_str())}, {5, std::atoi(out.at(1).c_str())}, {6, std::atoi(out.at(2).c_str())}};
        set_leds(_leds, versionmdode, wordclock_faceplate::WORD_COLOR_CLASS::MINUTE, _s);
    }
#endif

#ifdef WORDCLOCK_PCBREV
    const std::vector<std::tuple<int, int>> pcbver = {{7, WORDCLOCK_PCBREV}};
    set_leds(_leds, pcbver, wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT, _s);
#endif

#ifdef DEBUG
    const std::vector<std::tuple<int, int>> build = {{8, 1}};
#else
    const std::vector<std::tuple<int, int>> build = {{8, 2}};
#endif
    set_leds(_leds, build, wordclock_faceplate::WORD_COLOR_CLASS::DEFAULT, _s);
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

bool wordclock_faceplate::is_minutedot(const int _led_index)
{
    for (int i = 0; i < LED_MINUTEDOT_COUNT; i++)
    {
        if (LED_MINUTEDOT_POSITIONS[i] == _led_index)
        {
            return true;
        }
    }
    return false;
}



void wordclock_faceplate::set_leds(PicoLed::PicoLedController &_leds, const std::vector<std::tuple<int, int>> _word, const WORD_COLOR_CLASS _basecolor, const int _current_seconds)
{
    int led_to_set_index = -1;
    PicoLed::Color led_color;
    //  GET LED COLOR
    led_color = get_word_color_by_class(_basecolor, _current_seconds);

    // SET COLORS IN WORD
    for (int i = 0; i < _word.size(); i++)
    {
        const std::tuple<int, int> led_pos = _word.at(i);

        // GET LED INDEX
        if (std::get<0>(led_pos) == USE_DIRECT_LED_INDEXING)
        {
            led_to_set_index = std::get<1>(led_pos) % PICO_DEFAULT_WS2812_NUM;
        }
        else
        {
            // FLIP PIXEL IF NEEDED
            const std::tuple<int, int> index_flipped_final = flip_xy(led_pos, wordclock_faceplate::config.flip_state);
            led_to_set_index = xy_to_led_index(index_flipped_final);
        }

        // SEPERATE HANDLER FOR MINUTE DOTS
        // SIMPLY MIX COLOR WITH BLACK USING SEPERATE RATIOS
        if (is_minutedot(led_to_set_index))
        {
            led_color = PicoLed::MixColors(led_color, PicoLed::RGB(0, 0, 0), wordclock_faceplate::config.dotbrightness / 100.0); // MultiplyColors(led_color,  helper::limit((wordclock_faceplate::config.dotbrightness*1.0f / 100.0), 0, 100));
        }
        // FINALLY SET THE LED
        _leds.setPixelColor(led_to_set_index + LED_MATRIX_START_OFFSET, led_color);
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