#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include <PicoLed.hpp>
#include <Effects/Stars.hpp>

#include "wordclock_helper.h"

bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

#define BH1750_I2C_ADDR 0x23
#define BH1750_I2C_ADDR_ALT 0x5C
int enable_bh1750 = 0;

#define DS1307_I2C_ADDR 0x68
int enable_ds1307 = 0;


void init_i2c()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        if (addr % 16 == 0)
        {
            printf("%02x ", addr);
        }

        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr)){
            ret = PICO_ERROR_GENERIC;
        }else{
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
        }

        if (ret >= 0 && addr == BH1750_I2C_ADDR){
            enable_bh1750 = 1;
        }else if (ret >= 0 && addr == BH1750_I2C_ADDR_ALT){
            enable_bh1750 = 2;
        }else if(ret >= 0 && addr == DS1307_I2C_ADDR){
            enable_ds1307 = 1;
        }
    }
}

void init_bh1750()
{
    int addr = BH1750_I2C_ADDR;
    if (enable_bh1750 == 1)
    {
        addr = BH1750_I2C_ADDR;
    }
    else if (enable_bh1750 == 2)
    {
        addr = BH1750_I2C_ADDR_ALT;
    }

    uint8_t buf[1];

    buf[0] = 0x00;
    i2c_write_blocking(i2c_default, addr, buf, 1, true);
    buf[0] = 0x01;
    i2c_write_blocking(i2c_default, addr, buf, 1, true);
    buf[0] = 0x07;
    i2c_write_blocking(i2c_default, addr, buf, 1, true);
    buf[0] = 0x13;
    i2c_write_blocking(i2c_default, addr, buf, 1, true);
}

int imap(int x, const int in_min, const int in_max, const int out_min, const int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int get_brightness()
{

    int addr = BH1750_I2C_ADDR;
    if (enable_bh1750 == 1)
    {
        addr = BH1750_I2C_ADDR;
    }
    else if (enable_bh1750 == 2)
    {
        addr = BH1750_I2C_ADDR_ALT;
    }

    if (enable_bh1750 <= 0)
    {
        return 128;
    }
    int ret;

    uint8_t buf[2];

    if (reserved_addr(addr))
    {
        ret = PICO_ERROR_GENERIC;
    }
    else
    {
        ret = i2c_read_blocking(i2c_default, addr, buf, 2, false);
    }
    const uint16_t upper_byte = buf[0];
    const uint16_t lower_byte = buf[1];

    if (ret <= 0)
    {
        return 128;
    }

    const int lux = ((upper_byte << 8) + lower_byte) / (1.2 * 2);    
    return imap(lux, 0, 200, 10, 255);
}

void set_rtc(const int8_t _h, const int8_t _m, const int8_t _s)
{
    datetime_t t = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 0, // 0 is Sunday, so 5 is Friday
        .hour = _h,
        .min = _m,
        .sec = _s};
    rtc_set_datetime(&t);
    sleep_us(64);
}

void init_rtc()
{
    rtc_init();
}

datetime_t read_rtc()
{
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}

void update_display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    wordclock_helper::display_time(_leds, _h, _m, _s);
}

int main()
{
    stdio_init_all();

    // uart_init(PICO_DEFAULT_UART, PICO_DEFAULT_UART_BAUDRATE);
    // gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);
    init_rtc();
    set_rtc(0, 0, 0);
    init_i2c();
    init_bh1750();
    sleep_ms(1000);

    // 0. Initialize LED strip
    printf("0. Initialize LED strip\n");
    PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, PICO_DEFAULT_WS2812_PIN, PICO_DEFAULT_WS2812_NUM, PicoLed::FORMAT_GRB);
    

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // PicoLed::Stars effectStars(ledStrip, wordclock_helper::get_word_color(wordclock_helper::wordclock_word_index::M_FUENF, 0), 4.0);
    gpio_put(PICO_DEFAULT_LED_PIN, enable_ds1307);

    bool b = false;
    while (true)
    {
        datetime_t t = read_rtc();
        update_display_time(ledStrip, t.hour, t.min, t.sec);
        sleep_ms(500);

            
        ledStrip.setBrightness(get_brightness());
    }

    return 0;
}
