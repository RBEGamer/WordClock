/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// CUSTOM BOARD DEFINITION FOR WORDCLOCK PCB V1
// Initial Marcel Ochsendorf

#ifndef _BOARDS_WORDCLOCK_V1_RP2040_H
#define _BOARDS_WORDCLOCK_V1_RP2040_H

// For board detection
#define WORDCLOCK_PCBREV 1
#define WORDCLOCK_LANGUAGE 0 // see enum class FACEPLATES 0=GERMAN
#define WORDCLOCK_DISPlAY_ORIENTATION false // "0" or "1"
#define WORDCLOCK_BRIGHTNESS_MODE 0 // 0=auto 10-255 fixed
#define WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN 10  //IF IN AUTOMODE LED MIN BRIGHTNESS
#define WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX 200 //IF IN AUTOMODE LED MAXBRIGHTNESS
#define WORDCLOCK_BRIGHTNESS_MODE_AUTO_CURVE 15 // FLOAT FOR BRIGHTNESS CURVE
#define WORDCLOCK_COLOR_MODE 0 //see wordclock_faceplate.hpp 0=rainbow separate 2=white
#define WORDCLOCK_DAYLIGHTSAVING 1 //ENABLE EU DAYLIGHT SAVING


// CUSTOM PCB COMPONENTS
//ON V1 PCB THE RTC CHIP ISNT WOKRING SO WE USE THE BUILDIN RTC WITH NO BATTERY BACKUP
#define USE_RP2040RTC //USE RP2040 RTC IF NO OTHER TIMESOURCE IS PRESENT
//#define SET_INITIAL_TIME_ZERO // if set start time if no time sync happend to 0:0:0, otherwise to compiletime
//#define USE_I2CRTC_IF_PRESENT //USE PCF85623 IF PRESENT
//PCB v1 IS MISSING THE EEPROM SO  WE USE THE RP2040 FLASH
// THE USE_EEPROM_IF_PRESENT OPTION CAN OVERRIDE THIS, SO FLASH IS FALLBACK!
// PLEASE NOTE: SET PICO_FLASH_SIZE_BYTES (see below) CORRECTLY
#define USE_FLASH_AS_EEPROM
//PCB v2 OFFERS A M24C02 I2C EEPROM IF THIS IS PRESENT USE THIS TO STORE CONFIG
//#define USE_EEPROM_IF_PRESENT
//PCBv1 AND PCBv2 OFFERS A BH1750 LIGHT SENSOR TO SET LED BRIGHTNESS ACCORDING ROOM LIGHTNING
#define USE_BH1750_IF_PRESENT
// IF IN AUTOMODE ITS POSSIBLE TO REFRESH THE DISPLAY MORE OFTEN AND SHOW A SMOOTH BRIGHNESS TRANSISTION
// ONLY USABLE WITH USE_BH1750_IF_PRESENT OPTION
#define ENABLE_SMMOTH_FADE
//#define ENABLE_BLINKENDOTS
//#define ENABLE_UART_IRQ

// I2C DEVICE ADDRESSES

#define BH1750_I2C_ADDR 0x23 // BH1750 0x23 or 0x5C
#define RTC_I2C_ADDR 0x68 // (PCBv1)FOR DS1307/DS3231 0x68, (PCBv2)FOR PCF PCF85263AT 0x51
#define EEPROM_I2C_ADDR 0x50 //FOR m24c02 0x50, FOR at24c02 0x50

// On some samples, the xosc can take longer to stabilize than is usual
#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
#endif

//------------- UART -------------//
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#define PICO_DEFAULT_UART0
#endif

#ifndef PICO_DEFAULT_UART_BAUDRATE
#define PICO_DEFAULT_UART_BAUDRATE 9600
#endif

#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif

#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1
#endif

//------------- LED -------------//
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 25
#endif
//------------- WS2812 -------------//
// PiN 16 IF JP7 CLOSED
// PIN 22 IF JP6 CLOSED
#ifndef PICO_DEFAULT_WS2812_PIN
#define PICO_DEFAULT_WS2812_PIN 16
#endif

#define LED_MATRIX_START_OFFSET 1 // PCBv2 = 0,PCBv1 = 1 (on pcb v1 there is an additional led before the first matrix led)
#define LED_MATRIX_WIDTH 11
#define LED_MATRIX_HEIGHT 10
#define LED_MINUTEDOT_COUNT 4
#define LED_MINUTEDOT_POSITIONS (int[]){0, 12, 101, 113} //MINuTE DOT LED INDEXES
#define PICO_DEFAULT_WS2812_NUM ((LED_MATRIX_WIDTH*LED_MATRIX_HEIGHT)+ LED_MINUTEDOT_COUNT + LED_MATRIX_START_OFFSET)



//------------- I2C -------------//
#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 1
#endif

#ifndef PICO_DEFAULT_I2C_SPEED
#define PICO_DEFAULT_I2C_SPEED (100 * 1000)
#endif


#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 2
#endif

#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 3
#endif

//------------- SPI -------------//
#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 0
#endif

#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 19
#endif

#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 20
#endif

#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 18
#endif

//------------- FLASH -------------//

// Use slower generic flash access
// !!!! NEEDED !!!!!!!
#define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 4
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (8 * 1024 * 1024)
#endif


// All boards have B1 RP2040
#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 0
#endif

#endif
