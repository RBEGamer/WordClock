#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/flash.h" // for the flash erasing and writing
#include "hardware/sync.h"  // for the interrupts
#include "pico/unique_id.h"
#include <PicoLed.hpp>


#include "wifi_interface.h"
#include "settings_storage/settings_storage_include.h"
#include "helper.h"
#include "rtc/rtc_include.h"
#include "wordclock_faceplate/wordclock_faceplate_include.h"
#include "ambient_light/ambient_light_include.h"


void switch_fp(wordclock_faceplate *_instance, wordclock_faceplate::FACEPLATES _faceplate);
void init_bh1750(const int _i2_addr);
void init_eeprom_i2c(const int _i2_addr);
void init_rtc_i2c(const int _i2_addr);
void init_i2c();
void update_display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s);
void display_ip(PicoLed::PicoLedController &_leds, const std::string _ip);
void set_brightnesmode(const std::string _payload);
int set_faceplate(const int _fp);
void set_faceplate_str(const std::string _payload);
void set_displayorientation_str(const std::string _payload);
void set_displayorientation(const int _payload);
void set_time(const std::string _payload);
void set_date(const std::string _payload);
void set_dls(const std::string _payload);
void set_brightnesscurve(const std::string _payload);
void set_colormode(const std::string _payload);
int apply_brightnesscurve(const int _in);
void prepare_display_ip(const std::string _payload);
void restore_settings(bool _force);
void set_restoresettings(const std::string _payload);




#endif