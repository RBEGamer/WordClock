# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/board)


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build)


#set(PICO_BOARD "WORDCLOCK_V1_RP2040")
set(PICO_BOARD "WORDCLOCK_V1_RP2040_GERMAN_ROTATED")
#set(PICO_BOARD "WORDCLOCK_V2_RP2040")



# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(wordclock C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

include(${CMAKE_CURRENT_LIST_DIR}/PicoLED/PicoLed.cmake)


add_executable(wordclock 
        src/settings_storage.hpp
        src/settings_storage.cpp
        src/settings_storage_flash.hpp
        src/settings_storage_flash.cpp
        src/settings_storage_eeprom.hpp
        src/settings_storage_eeprom.cpp
        src/settings_storage/settings_storage.hpp
        src/settings_storage/settings_storage.cpp
        src/settings_storage/settings_storage_flash.hpp
        src/settings_storage/settings_storage_flash.cpp
        src/settings_storage/settings_storage_eeprom.hpp
        src/settings_storage/settings_storage_eeprom.cpp
        src/programinfo.h
        src/wordclock_faceplate_include.h
        src/wordclock_faceplate_german.cpp
        src/wordclock_faceplate_german.hpp
        src/wordclock_faceplate_english.cpp
        src/wordclock_faceplate_english.hpp
        src/wordclock_faceplate_binary.cpp
        src/wordclock_faceplate_binary.hpp
        src/wordclock_faceplate_dots.cpp
        src/wordclock_faceplate_dots.hpp
        src/rtc.h
        src/rtc.cpp
        src/wordclock_faceplate/wordclock_faceplate_include.h
        src/wordclock_faceplate/wordclock_faceplate_german.cpp
        src/wordclock_faceplate/wordclock_faceplate_german.hpp
        src/wordclock_faceplate/wordclock_faceplate_english.cpp
        src/wordclock_faceplate/wordclock_faceplate_english.hpp
        src/wordclock_faceplate/wordclock_faceplate_binary.cpp
        src/wordclock_faceplate/wordclock_faceplate_binary.hpp
        src/wordclock_faceplate/wordclock_faceplate_dots.cpp
        src/wordclock_faceplate/wordclock_faceplate_dots.hpp
        src/wordclock_faceplate/wordclock_faceplate.hpp
        src/wordclock_faceplate/wordclock_faceplate.cpp
        src/wordclock_faceplate/wordclock_faceplate_constants.h
        src/rtc/rtc.h
        src/rtc/rtc.cpp
        src/helper.h
        src/helper.cpp
        src/wordclock_faceplate.hpp

        src/wifi_interface.h
        src/wifi_interface.cpp
        src/main.cpp
        src/wordclock_faceplate.cpp
        src/wordclock_faceplate_constants.h

        )

pico_set_program_name(wordclock "wordclock")
pico_set_program_version(wordclock "1.0")
pico_set_program_url(wordclock "https://marcelochsendorf.com")
pico_set_program_description(wordclock "software for a rp2040 base 10x11 wordclock")

pico_enable_stdio_uart(wordclock 0)
pico_enable_stdio_usb(wordclock 1)

target_link_libraries(wordclock 
        pico_stdlib
        hardware_pio
        hardware_rtc
        hardware_i2c
        hardware_adc
        hardware_flash
        hardware_sync
)



# Add the standard library to the build
target_link_libraries(wordclock pico_stdlib PicoLed)

pico_add_extra_outputs(wordclock)

