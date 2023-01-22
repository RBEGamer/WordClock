#ifndef __WORDCLOCK_FACEPLATE_ITALIAN_H__
#define __WORDCLOCK_FACEPLATE_ITALIAN_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "wordclock_faceplate.hpp"

class wordclock_faceplate_italian : public wordclock_faceplate
{

    enum class WORDS_INDEX
    {
        WORDS_INDEX_COMMON_BEGIN, // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        C_SONO,
        C_LE,
        C_ORE,
        C_E,
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END,   // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_OE,
        M_MENO,
        M_CINIQUE,
        M_DIECI,
        M_QUINDICI,
        M_VENTI,
        M_TRENTA,
        M_UN,
        M_MEZZA,
        M_QUARTO,
        //---- MINUTE WORDS END------ //
        //---- MINUTE DOTS START------ //
        M_DOT_ONE,
        M_DOT_TWO,
        M_DOT_THREE,
        M_DOT_FOUR,
        //---- MINUTE DOTS END------ //
        WORDS_INDEX_MINUTE_END, // DONT DELETE
        WORDS_INDEX_HOUR_BEGIN, // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        H_UNA,
        H_DUE,
        H_TRE,
        H_QUATTRO,
        H_CINQUE,
        H_SEI,
        H_SETTE,
        H_OTTO,
        H_NOVE,
        H_DIECI,
        H_UNDICI,
        H_DODICI,
        //---- HOUR WORDS END------ //
        WORDS_INDEX_HOUR_END, // DONT DELETE
        LENGHT                // DONT DELETE
    };                        // 27

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    static inline const std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)] = {
        {},                                                         // DONT DELETE
        //------------ COMMON WORDS BEGIN ------------------------- //
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},                           // C_SONO
        {{0, 5}, {0, 6}},                                           // C_LE
        {{0, 8}, {0, 9}, {0, 10}},                                  // C_ORE
        {{7, 0}},                                                   // M_OE => M_E
        //----------- COMMON WORDS END ---------------------------- //
        {},                                                         // DONT DELETE
        {},                                                         // DONT DELETE
                                                                    //---- MINUTE WORDS BEGIN------ //
        {{7, 0}},                                                   // M_E
        {{6, 7}, {6, 8}, {6, 9}, {6, 10}},                          // M_MENO
        {{8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9}, {8, 10}},          // M_CINIQUE
        {{9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}},                   // M_DIECI
        {},                                                         // M_QUINDICI
        {{8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}},                   // M_VENTI
        {},                                                         // M_TRENTA
        {{7, 2}, {7, 3}},                                           // M_UN
        {{9, 6}, {9, 7}, {9, 8}, {9, 9}, {9, 10}},                  // M_MEZZA
        {{7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {7, 10}},          // M_QUARTO
        //-------- MINUTE DOTS ------------------------------------ //
        // USE DIRECT LED INDEXING FOR MINUTE DOTS, BECAUSE THESE ARE NOT IN THE MATRIX SPACE
        // SEE BOARD DEFINITION FOR ACTUAL VALUES AND DEFINITION => PCB VERSION DEPENDEND
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[0]}},    // M_DOT_ONE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[1]}},    // M_DOT_TWO
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[2]}},    // M_DOT_THREE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[3]}},    // M_DOT_FOUR
        //------ MINUTE WORDS END --------------------------------- //
        {},                                                         // DONT DELETE
        {},                                                         // DONT DELETE
        //------ HOUR WORDS BEGIN --------------------------------- //
        {{1, 3}, {1, 4}, {1, 5}},                                   // H_UNA
        {{1, 7}, {1, 8}, {1, 9}},                                   // H_DUE
        {{2, 0}, {2, 1}, {2, 2}},                                   // H_TRE
        {{5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}},   // H_QUATTRO
        {{6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}},           // H_CINQUE
        {{5, 8}, {5, 9}, {5, 10}},                                  // H_SEI
        {{4, 6}, {4, 7}, {4, 8}, {4, 9}, {4, 10}},                  // H_SETTE
        {{2, 3}, {2, 4}, {2, 5}, {2, 6}},                           // H_OTTO
        {{2, 7}, {2, 8}, {2, 9}, {2, 10}},                          // H_NOVE
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}},                   // H_DIECI
        {{3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10}},          // H_UNDICI
        {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}},           // H_DODICI
        //----- HOUR WORDS END ------------------------------------ //
        {} // DONT DELETE
    };

    void set_word(PicoLed::PicoLedController &_leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_italian();
    ~wordclock_faceplate_italian();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;
};
#endif
