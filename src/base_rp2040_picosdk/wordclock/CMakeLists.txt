# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/board)


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build)


set(PICO_BOARD "WORDCLOCK_V1_RP2040")
# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(wordclock C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

include("PicoLed/PicoLed.cmake")

set(HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/include/wordclock_helper.h)
add_executable(wordclock src/wordclock_helper.h src/main.cpp src/wordclock_helper.cpp)

pico_set_program_name(wordclock "wordclock")
pico_set_program_version(wordclock "0.9")

pico_enable_stdio_uart(wordclock 0)
pico_enable_stdio_usb(wordclock 1)

target_link_libraries(wordclock 
        pico_stdlib hardware_pio hardware_rtc hardware_i2c
)


# MOVED TO BOARD DEFINITIoN
# https://forums.raspberrypi.com/viewtopic.php?t=313306
#target_compile_definitions(wordclock PRIVATE PICO_FLASH_SPI_CLKDIV=4)
#target_compile_definitions(wordclock PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

# Add the standard library to the build
target_link_libraries(wordclock pico_stdlib PicoLed)

pico_add_extra_outputs(wordclock)

