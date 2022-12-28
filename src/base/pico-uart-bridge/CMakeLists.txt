# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk)
include(pico_sdk_import.cmake)

project(pico_uart_bridge C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_executable(uart_bridge uart-bridge.c usb-descriptors.c)

target_include_directories(uart_bridge PUBLIC
	./
	pico-sdk/lib/tinyusb/src)

target_link_libraries(uart_bridge
	hardware_flash
	pico_multicore
	pico_stdlib
	tinyusb_device)

pico_add_extra_outputs(uart_bridge)
