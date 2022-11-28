#ifndef __WORDCLOCK_FACEPLATE_BINARY_H__
#define __WORDCLOCK_FACEPLATE_BINARY_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "helper.h"
#include "wordclock_faceplate.hpp"



class wordclock_faceplate_binary : public wordclock_faceplate
{

    enum class WORDS_INDEX
    {
        WORDS_INDEX_COMMON_BEGIN, // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END, // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_ONES_ONE,
        M_ONES_TWO,
        M_ONES_FOUR,
        M_ONES_EIGHT,
        M_TENS_ONE,
        M_TENS_TWO,
        M_TENS_FOUR,
        M_TENS_EIGHT,
        //---- MINUTE WORDS END------ //
        WORDS_INDEX_MINUTE_END, // DONT DELETE
        WORDS_INDEX_HOUR_BEGIN, // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        H_ONES_ONE,
        H_ONES_TWO,
        H_ONES_FOUR,
        H_ONES_EIGHT,
        H_TENS_ONE,
        H_TENS_TWO,
        H_TENS_FOUR,
        H_TENS_EIGHT,
        //---- HOUR WORDS END------ //
        WORDS_INDEX_HOUR_END, // DONT DELETE
        LENGHT // DONT DELETE
    }; //27


    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)  
    static inline const std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)] = {
        {},  // DONT DELETE
         //---- COMMON WORDS BEGIN------ //
        //---- COMMON WORDS END------ //
        {},  // DONT DELETE
        {},  // DONT DELETE
         //---- MINUTE WORDS BEGIN------ //
        {{8, 9}, {8, 10}, {7, 9}, {7, 10}},                     // M_ONES_ONE
        {{6, 9}, {6, 10}, {5, 9}, {5, 10}},                     // M_ONES_TWO
        {{4, 9}, {4, 10}, {3, 9}, {3, 10}},                     // M_ONES_FOUR
        {{2, 9}, {2, 10}, {1, 9}, {1, 10}},                     // M_ONES_EIGHT
        {{8, 6}, {8, 7}, {7, 6}, {7, 7}},                       // M_TENS_ONW
        {{6, 6}, {6, 7}, {5, 6}, {5, 7}},                       // M_TENS_TWO
        {{4, 6}, {4, 7}, {3, 6}, {3, 7}},                       // M_TENS_FOUR
        {{2, 6}, {2, 7}, {1, 6}, {1, 7}},                       // M_TENS_EIGHT
         //---- MINUTE WORDS END------ //
        {},  // DONT DELETE
        {},  // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        {{8, 3}, {8, 4}, {7, 3}, {7, 4}},                     // H_ONES_ONE
        {{6, 3}, {6, 4}, {5, 3}, {5, 4}},                     // H_ONES_TWO
        {{4, 3}, {4, 4}, {3, 3}, {3, 4}},                     // H_ONES_FOUR
        {{2, 3}, {2, 4}, {1, 3}, {1, 4}},                     // H_ONES_EIGHT
        {{8, 0}, {8, 1}, {7, 0}, {7, 1}},                       // H_TENS_ONW
        {{6, 0}, {6, 1}, {5, 0}, {5, 1}},                       // H_TENS_TWO
        {{4, 0}, {4, 1}, {3, 0}, {3, 1}},                       // H_TENS_FOUR
        {{2, 0}, {2, 1}, {1, 0}, {1, 1}},                       // H_TENS_EIGHT
        //---- HOUR WORDS END------ //
        {}  // DONT DELETE                          
    };


    void set_word(PicoLed::PicoLedController& _leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_binary();
    ~wordclock_faceplate_binary();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;

};
#endif
