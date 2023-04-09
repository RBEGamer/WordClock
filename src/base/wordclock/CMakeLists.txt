cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
        message(WARNING "NO CMAKE_BUILD_TYPE SET")
        set(CMAKE_BUILD_TYPE "Release")
endif()

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/board)

if(WORDCLOCK_BOARD)
        set(PICO_BOARD "${WORDCLOCK_BOARD}")
else()
        message(WARNING "WORDCLOCK_BOARD NOT SET")      
        set(PICO_BOARD "WORDCLOCK_V2_RP2040_RP2040RTC")
endif()



set(VERSION "4.0.0")





# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(wordclock C CXX ASM)


set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()




include(${CMAKE_CURRENT_LIST_DIR}/PicoLED/PicoLed.cmake)

add_executable(wordclock 
        src/ambient_light/ambient_light_include.h
        src/ambient_light/ambient_light.hpp
        src/ambient_light/ambient_light.cpp
        src/ambient_light/ambient_light_bh1750.hpp
        src/ambient_light/ambient_light_bh1750.cpp
        src/settings_storage/settings_storage_include.h
        src/settings_storage/settings_storage.hpp
        src/settings_storage/settings_storage.cpp
        src/settings_storage/settings_storage_flash.hpp
        src/settings_storage/settings_storage_flash.cpp
        src/settings_storage/settings_storage_eeprom.hpp
        src/settings_storage/settings_storage_eeprom.cpp
        src/programinfo.h
        src/wordclock_faceplate/wordclock_faceplate_include.h
        src/wordclock_faceplate/wordclock_faceplate_german.cpp
        src/wordclock_faceplate/wordclock_faceplate_german.hpp
        src/wordclock_faceplate/wordclock_faceplate_italian.cpp
        src/wordclock_faceplate/wordclock_faceplate_italian.hpp
        src/wordclock_faceplate/wordclock_faceplate_english.cpp
        src/wordclock_faceplate/wordclock_faceplate_english.hpp
        src/wordclock_faceplate/wordclock_faceplate_binary.cpp
        src/wordclock_faceplate/wordclock_faceplate_binary.hpp
        src/wordclock_faceplate/wordclock_faceplate_dots.cpp
        src/wordclock_faceplate/wordclock_faceplate_dots.hpp
        src/wordclock_faceplate/wordclock_faceplate.hpp
        src/wordclock_faceplate/wordclock_faceplate.cpp
        src/wordclock_faceplate/wordclock_faceplate_constants.h
        src/rtc/rtc.hpp
        src/rtc/rtc.cpp
        src/rtc/rtc_rp2040.hpp
        src/rtc/rtc_rp2040.cpp
        src/rtc/rtc_include.h
        src/rtc/rtc_i2c.cpp
        src/rtc/rtc_i2c.hpp
        src/helper.h
        src/helper.cpp
        src/wifi_interface.hpp
        src/wifi_interface.cpp
        src/main.h
        src/main.cpp
        )


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(wordclock PRIVATE DEBUG=1)
        message(STATUS "ADD DEBUG=1")
else()
        target_compile_definitions(wordclock PRIVATE DEBUG=0)
endif()

target_compile_definitions(wordclock PRIVATE VERSION="${VERSION}")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        #set(CMAKE_CXX_FLAGS "-Wall")
        set(CMAKE_CXX_FLAGS_DEBUG "-g")
        message(STATUS "RELEASE BUILD")
endif()


pico_set_program_name(wordclock "${PICO_BOARD}_${CMAKE_BUILD_TYPE}")
pico_set_program_version(wordclock "${VERSION}")
pico_set_program_url(wordclock "https://marcelochsendorf.com")
pico_set_program_description(wordclock "software for a rp2040 base 10x11 wordclock")
pico_enable_stdio_uart(wordclock 0)
pico_enable_stdio_usb(wordclock 1)

set_target_properties(wordclock PROPERTIES OUTPUT_NAME "${PICO_BOARD}_${VERSION}_${CMAKE_BUILD_TYPE}")

target_link_libraries(wordclock 
        pico_stdlib
        hardware_pio
        hardware_rtc
        hardware_i2c
        hardware_adc
        hardware_flash
        hardware_sync
        hardware_timer
)



# Add the standard library to the build
target_link_libraries(wordclock pico_stdlib PicoLed)

pico_add_extra_outputs(wordclock)

