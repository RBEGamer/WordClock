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
        //---- COMMON WORDS END------ //
        WORDS_INDEX_COMMON_END, // DONT DELETE
        WORDS_INDEX_MINUTE_BEGIN, // DONT DELETE
        //---- MINUTE WORDS BEGIN------ //
        M_FUENF,
        M_ZEHN,
        M_ZWANZIG,
        M_VIERTEL,
        M_VOR,
        M_NACH,
        M_HALB,
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
        LENGHT // DONT DELETE
    }; //27


    // INLINE FOR IN-CLASS INITIALISATION
    static inline const std::vector<std::tuple<int, int>> WORDCLOCKwORDS[((int)WORDS_INDEX::LENGHT)-6] = {
        //-------- COMMON WORDS ------------- //
        {{0, 0}, {0, 1}},                   // C_ES
        {{0, 2}, {0, 3}, {0, 4}},           // C_IST
        {},               // C_UHR
        //-------- MINUTES ----------------- //
        {},                                 // M_FUENF
        {},                                 // M_ZEHN
        {},                                 // M_ZWANZIG
        {},                                 // M_VIERTEL
        {},                                 // M_VOR
        {},                                 // M_NACH
        {},                                 // M_HALB
        //-------- MINUTE DONTS ------------- //
        // USE DIRECT LED INDEXING FOR MINUTE DOTS, BECAUSE THESE ARE NOT IN THE MATRIX SPACE
        {{USE_DIRECT_LED_INDEXING, 0}},     // M_DOT_ONE
        {{USE_DIRECT_LED_INDEXING, 12}},    // M_DOT_TWO
        {{USE_DIRECT_LED_INDEXING, 101}},   // M_DOT_THREE
        {{USE_DIRECT_LED_INDEXING, 113}},   // M_DOT_FOUR
        //-------- HOURS ------------- //
        {},                                 // H_EINS
        {},                                 // H_ZWEI
        {},                                 // H_DREI
        {},                                 // H_VIER
        {},                                 // H_FUENF
        {},                                 // H_SECHS
        {},                                 // H_SIEBEN
        {},                                 // H_ACHT
        {},                                 // H_NEUN
        {},                                 // H_ZEHN
        {},                                 // H_ELF
        {},                                 // H_ZWOELF
        {}                                  // H_EIN
    };



    void set_word(PicoLed::PicoLedController& _leds, const WORDS_INDEX _word, const int _current_seconds);

public:
    wordclock_faceplate_german();
    ~wordclock_faceplate_german();

    
    void display_time_with_words(PicoLed::PicoLedController& _leds, const int _horig, const int _m, const int _s) override;

};
#endif