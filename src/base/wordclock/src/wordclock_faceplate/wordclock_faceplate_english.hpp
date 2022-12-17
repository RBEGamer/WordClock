#ifndef __WORDCLOCK_FACEPLATE_ENGLISH_H__
#define __WORDCLOCK_FACEPLATE_ENGLISH_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "wordclock_faceplate.hpp"

class wordclock_faceplate_english : public wordclock_faceplate
{

    enum class WORDS_INDEX
    {
        WORDS_INDEX_COMMON_BEGIN, // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        C_IT,
        C_IS,
        C_OCLOCK,
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END,   // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_QUATER,
        M_TWENTY,
        M_FIVE,
        M_TEN,
        M_TO,
        M_HALF,
        M_PAST,
        
        M_DOT_ONE,
        M_DOT_TWO,
        M_DOT_THREE,
        M_DOT_FOUR,
        //---- MINUTE WORDS END------ //
        WORDS_INDEX_MINUTE_END, // DONT DELETE
        WORDS_INDEX_HOUR_BEGIN, // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        H_NINE,
        H_ONE,
        H_SIX,
        H_THREE,
        H_FOUR,
        H_FIVE,
        H_TWO,
        H_EIGHT,
        H_ELEVEN,
        H_SEVEN,
        H_TWELVE,
        H_TEN,
        H_AM,
        H_PM,
        //---- HOUR WORDS END------ //
        WORDS_INDEX_HOUR_END, // DONT DELETE
        LENGHT                // DONT DELETE
    };                        // 27

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    static inline const std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)] = {
        {},                                                         // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        {{0, 0}, {0, 1}},                                           // C_IT
        {{0, 3}, {0, 4}},                                           // C_IS
        {{9, 5}, {9, 6}, {9, 7}, {9, 8}, {9, 9}, {9, 10}},          // C_OCLOCK
        //---- COMMON WORDS END------ //
        {},                                                         // DONT DELETE
        {},                                                         // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        {{1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}},   // M_QUATER
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}},           // M_TWENTY
        {{2, 6}, {2, 7}, {2, 8}, {2, 9}},                           // M_FIVE
        {{3, 5}, {3, 6}, {3, 7}},                                   // M_TEN
        {{3, 9}, {3, 10}},                                          // M_TO
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}},                           // M_HALF
        {{4, 0}, {4, 1}, {4, 2}, {4, 3}},                           // M_PAST
        //-------- MINUTE DOTS ------------------------------- //
        // USE DIRECT LED INDEXING FOR MINUTE DOTS, BECAUSE THESE ARE NOT IN THE MATRIX SPACE
        // SEE BOARD DEFINITION FOR ACTUAL VALUES AND DEFINITION => PCB VERSION DEPENDEND
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[0]}},    // M_DOT_ONE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[1]}},    // M_DOT_TWO
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[2]}},    // M_DOT_THREE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[3]}},    // M_DOT_FOUR
        //---- MINUTE WORDS END------ //
        {},                                                         // DONT DELETE
        {},                                                         // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        {{4, 7}, {4, 8}, {4, 9}, {4, 10}},                           // H_NINE
        {{5, 0}, {5, 1}, {5, 2}},                                   // H_ONE
        {{5, 3}, {5, 4}, {5, 5}},                                   // H_SIX
        {{5, 6}, {5, 7}, {5, 8}, {5, 9}, {5, 10}},                  // H_THREE
        {{6, 0}, {6, 1}, {6, 2}, {6, 3}},                           // H_FOUR
        {{6, 4}, {6, 5}, {6, 6}, {6, 7}},                           // H_FIVE
        {{6, 8}, {6, 9}, {6, 10}},                                  // H_TWO
        {{7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}},                   // H_EIGHT
        {{7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {7, 10}},          // H_ELEVEN
        {{8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}},                   // H_SEVEN
        {{8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9}, {8, 10}},          // H_TWELVE
        {{9, 0}, {9, 1}, {9, 2}},                                   // H_TEN
        {{0, 7}, {0, 8}},                                           // H_AM
        {{0, 9}, {0, 10}},                                          // H_PM
        //---- HOUR WORDS END------ //
        {} // DONT DELETE
    };

    void set_word(PicoLed::PicoLedController &_leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_english();
    ~wordclock_faceplate_english();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;
};
#endif
