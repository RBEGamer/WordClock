// TO CREATE A NEW TEMPLATE COPY .cpp and .hpp and rename wordclock_faceplate_template => wordclock_faceplate_<LANGUAGE>
// add wordclock_faceplate_<LANGUAGE>.hpp into  wordclock_faceplate_include.h
// add <LANGUAGE> to the enum class FACEPLATES in wordclock_faceplate.hpp
// if you want dynamic switching implement void switch_fp(wordclock_faceplate *_instance, wordclock_faceplate::FACEPLATES _faceplate) in main.cpp
#ifndef __WORDCLOCK_FACEPLATE_TEMPLATE_H__
#define __WORDCLOCK_FACEPLATE_TEMPLATE_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "wordclock_faceplate.hpp"

class wordclock_faceplate_template : public wordclock_faceplate
{
    // THE ENUM CONTAINS THE WORDS IN THE FONTFACE
    // WORDS_INDEX_COMMON_BEGIN / WORDS_INDEX_COMMON_END
    // MARKERS ARE USED TO COLOR THE WORD CATEGORY (MINUTES/HOUR/MISC/SECONDS) IN DIFFERENT COLORS
    // ITS POSSIBLE TO IMPLEMENT OWN CUSTOM COLOR CODE IN THE set_word FUNCTION
    // USE  wordclock_faceplate_template::set_word(_leds, wordclock_faceplate_template::WORDS_INDEX::<WORD>, _s); TO DRAW THE WORD
    enum class WORDS_INDEX
    {
        WORDS_INDEX_COMMON_BEGIN, // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        C_ES,
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END,   // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_FUENF,
        //---- MINUTE WORDS END------ //
        WORDS_INDEX_MINUTE_END, // DONT DELETE
        WORDS_INDEX_HOUR_BEGIN, // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        H_EINS,
        //---- HOUR WORDS END------ //
        WORDS_INDEX_HOUR_END, // DONT DELETE
        LENGHT                // DONT DELETE
    };                        // 27

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    // SO IF YOU WANT TO LIGHT UP THE FIRST TWO LEDS IN THE TOP-LEFT CORNER USE:
    // {{0, 0}, {0, 1}}


    // THE INDEX MATCHES THE ORDER IN THE WORDS_INDEX ENUM
    // IF YOU WANT TO ADDRESS AN LED VIA ITS INDEX POSITON IN THE CHAIN USE
    //         {{USE_DIRECT_LED_INDEXING, <LED_INDEX>}}

    //THIS IS USE FOR THE LEDS OUTSIDE OF THE MATRIX SUCH AS THE MINUTE DOT LED IN THE CORNERS
    //    {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[0]}}, // TOP LEFT
    //    {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[1]}}, // TOP RIGHT
    //    {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[2]}}, // BOTTOM_RIGHT
    //    {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[3]}}, // BOTTOM_LEFT
    static inline const std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)] = {
        {},                               // DONT DELETE
                                          //---- COMMON WORDS BEGIN------ //
        {{0, 0}, {0, 1}},                 // C_ES
        
        //---- COMMON WORDS END------ //
        {},                                                        // DONT DELETE
        {},                                                        // DONT DELETE
                                                                   //---- MINUTE WORDS BEGIN------ //
        {{0, 7}, {0, 8}, {0, 9}, {0, 10}},                         // M_FUENF
                                                                    //---- MINUTE WORDS END------ //
        {},                                                      // DONT DELETE
        {},                                                      // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        {{5, 0}, {5, 1}, {5, 2}, {5, 3}},                 // H_EINS
        //---- HOUR WORDS END------ //
        {} // DONT DELETE
    };

    void set_word(PicoLed::PicoLedController &_leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_template();
    ~wordclock_faceplate_template();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;
};
#endif
