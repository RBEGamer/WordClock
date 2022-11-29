#ifndef __WORDCLOCK_FACEPLATE_GERMAN_H__
#define __WORDCLOCK_FACEPLATE_GERMAN_H__

#include <PicoLed.hpp>
#include <vector>
#include <tuple>

#include "wordclock_faceplate.hpp"

class wordclock_faceplate_german : public wordclock_faceplate
{

    enum class WORDS_INDEX
    {
        WORDS_INDEX_COMMON_BEGIN, // DONT DELETE
        //---- COMMON WORDS BEGIN------ //
        C_ES,
        C_IST,
        C_UHR,
        C_FUNK,
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END,   // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_FUENF,
        M_ZEHN,
        M_ZWANZIG,
        M_VIERTEL,
        M_VOR,
        M_NACH,
        M_HALB,
        M_DREI,
        M_DOT_ONE,
        M_DOT_TWO,
        M_DOT_THREE,
        M_DOT_FOUR,
        //---- MINUTE WORDS END------ //
        WORDS_INDEX_MINUTE_END, // DONT DELETE
        WORDS_INDEX_HOUR_BEGIN, // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        H_EINS,
        H_ZWEI,
        H_DREI,
        H_VIER,
        H_FUENF,
        H_SECHS,
        H_SIEBEN,
        H_ACHT,
        H_NEUN,
        H_ZEHN,
        H_ELF,
        H_ZWOELF,
        H_EIN,
        //---- HOUR WORDS END------ //
        WORDS_INDEX_HOUR_END, // DONT DELETE
        LENGHT                // DONT DELETE
    };                        // 27

    // INLINE FOR IN-CLASS INITIALISATION
    // ARRAY WITH LED LOCATION FOR EACH WORD {{ROW, COLUMN} starting top/left corner (near battery)
    static inline const __in_flash() std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)] = {
        {},                               // DONT DELETE
                                          //---- COMMON WORDS BEGIN------ //
        {{0, 0}, {0, 1}},                 // C_ES
        {{0, 3}, {0, 4}, {0, 5}},         // C_IST
        {{9, 8}, {9, 9}, {9, 10}},        // C_UHR
        {{3, 3}, {3, 4}, {3, 5}, {3, 6}}, // C_FUNK
        //---- COMMON WORDS END------ //
        {},                                                        // DONT DELETE
        {},                                                        // DONT DELETE
                                                                   //---- MINUTE WORDS BEGIN------ //
        {{0, 7}, {0, 8}, {0, 9}, {0, 10}},                         // M_FUENF
        {{1, 0}, {1, 1}, {1, 2}, {1, 3}},                          // M_ZEHN
        {{1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}}, // M_ZWANZIG
        {{2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10}}, // M_VIERTEL
        {{3, 0}, {3, 1}, {3, 2}},                                  // M_VOR
        {{3, 7}, {3, 8}, {3, 9}, {3, 10}},                         // M_NACH
        {{4, 0}, {4, 1}, {4, 2}, {4, 3}},                          // M_HALB
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}},                          // M_DREI
        //-------- MINUTE DOTS ------------------------------- //
        // USE DIRECT LED INDEXING FOR MINUTE DOTS, BECAUSE THESE ARE NOT IN THE MATRIX SPACE
        // SEE BOARD DEFINITION FOR ACTUAL VALUES AND DEFINITION => PCB VERSION DEPENDEND
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[0]}}, // M_DOT_ONE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[1]}}, // M_DOT_TWO
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[2]}}, // M_DOT_THREE
        {{USE_DIRECT_LED_INDEXING, LED_MINUTEDOT_POSITIONS[3]}}, // M_DOT_FOUR
                                                                 //---- MINUTE WORDS END------ //
        {},                                                      // DONT DELETE
        {},                                                      // DONT DELETE
        //---- HOUR WORDS BEGIN------ //
        {{5, 0}, {5, 1}, {5, 2}, {5, 3}},                 // H_EINS
        {{5, 7}, {5, 8}, {5, 9}, {5, 10}},                // H_ZWEI
        {{6, 0}, {6, 1}, {6, 2}, {6, 3}},                 // H_DREI
        {{6, 7}, {6, 8}, {6, 9}, {6, 10}},                // H_VIER
        {{4, 7}, {4, 8}, {4, 9}, {4, 10}},                // H_FUENF
        {{7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}},         // H_SECHS
        {{8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}}, // H_SIEBEN
        {{7, 7}, {7, 8}, {7, 9}, {7, 10}},                // H_ACHT
        {{9, 3}, {9, 4}, {9, 5}, {9, 6}},                 // H_NEUN
        {{9, 0}, {9, 1}, {9, 2}, {9, 3}},                 // H_ZEHN
        {{4, 5}, {4, 6}, {4, 7}},                         // H_ELF
        {{8, 6}, {8, 7}, {8, 8}, {8, 9}, {8, 10}},        // H_ZWOELF
        {{5, 0}, {5, 1}, {5, 2}},                         // H_EIN
        //---- HOUR WORDS END------ //
        {} // DONT DELETE
    };

    void set_word(PicoLed::PicoLedController &_leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_german();
    ~wordclock_faceplate_german();
    void display_time_with_words(PicoLed::PicoLedController &_leds, const int _horig, const int _m, const int _s) override;
};
#endif
