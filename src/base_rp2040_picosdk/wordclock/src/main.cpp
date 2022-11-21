#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"


#include <PicoLed.hpp>
#include <Effects/Stars.hpp>


#include "wifi_interface.h"
#include "wordclock_helper.h"
#include "helper.h"
#include "rtc.h"


//STORES IF i2C devices were found
int enable_bh1750_addr = -1;
int enable_ds1307_addr = -1;

void init_i2c()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        int ret;
        uint8_t rxdata;
        if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78)
        {
            ret = PICO_ERROR_GENERIC;
        }
        else
        {
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
            
        }

        if (ret > 0){
                printf("i2c device at %02x ", addr);
        }

        // CHECK FOR REQUESTED DEVIES FOUND
        if (ret >= 0 && addr == BH1750_I2C_ADDR)
        {
            enable_bh1750_addr = addr;
        }
        else if (ret >= 0 && addr == BH1750_I2C_ADDR_ALT)
        {
            enable_bh1750_addr = addr;
        }
        else if (ret >= 0 && addr == DS1307_I2C_ADDR)
        {
            enable_ds1307_addr = addr;
        }
    }
}

void init_bh1750()
{
    if (enable_bh1750_addr < 0)
    {
        printf("init_bh1750 failed");
        return;
    }

    // INIT SEQUENCE POWER DOWN, POWER UP, RESET, CONT READING MODE
    uint8_t buf[1];
    buf[0] = 0x00;
    i2c_write_blocking(i2c_default, enable_bh1750_addr, buf, 1, true);
    buf[0] = 0x01;
    i2c_write_blocking(i2c_default, enable_bh1750_addr, buf, 1, true);
    buf[0] = 0x07;
    i2c_write_blocking(i2c_default, enable_bh1750_addr, buf, 1, true);
    buf[0] = 0x13;
    i2c_write_blocking(i2c_default, enable_bh1750_addr, buf, 1, true);
}


int get_brightness()
{
    // IF NO MODULE FOUND
    if (enable_bh1750_addr < 0)
    {
        return -1;
    }
    int ret;

    uint8_t buf[2];   
    ret = i2c_read_blocking(i2c_default, enable_bh1750_addr, buf, 2, false);

    const uint16_t upper_byte = buf[0];
    const uint16_t lower_byte = buf[1];
    
    if (ret <= 0)
    {
        return -1;
    }

    const int lux = ((upper_byte << 8) + lower_byte) / (1.2 * 2); // custom 1.5 factor added
    return lux;
}

const int num_readings = 30;
int readings[num_readings];
int reading_index = 0;
long avg_sum = 0;

int get_average_brightness()
{

    long average;
    // subtract the last reading:
    avg_sum = avg_sum - readings[reading_index];
    // read the sensor:
    const int b = get_brightness();
   
    if (b >= 0)
    {
        readings[reading_index] = b; 
    }

    // add value to total:
    avg_sum = avg_sum + readings[reading_index];
    // handle index
    reading_index = reading_index + 1;
    if (reading_index >= num_readings)
    {
        reading_index = 0;
    }
    average = avg_sum / num_readings;
    // 0-420lux => 0-255 led brightness and limit range
    const int r = std::max(5, std::min(255, helper::map((int)average, 0, 420, 5, 255)));
    return r;
}

void update_display_time(PicoLed::PicoLedController &_leds, const int _h, const int _m, const int _s)
{
    wordclock_helper::display_time(_leds, _h, _m, _s);
}



int main()
{
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, true);



    wifi_interface::init_uart();
    sleep_ms(2000); //WAIT FOR UART / USB A BIT 
    printf("START");
    rtc::init_rtc();
    rtc::set_rtc_time(__TIME__);
    init_i2c();
    init_bh1750();
    

    //modified lib for 400khz
    PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, PICO_DEFAULT_WS2812_PIN, PICO_DEFAULT_WS2812_NUM, PicoLed::FORMAT_GRB);



    int current_brightness = get_average_brightness();
    int current_brightness_mode = 0; //0=auto >0-255 = manual
    int last_brightness = 0;
    int last_tmin = -1;
    int last_tsec = -1;

    //enable uart rx irq for communication with wifi module
    //wifi_interface::enable_uart_irq(true);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    while (true)
    {
        
        // UPDATE DISPLAY IF NEEDED
        datetime_t t = rtc::read_rtc();
        if(last_tsec != t.sec){
            last_tsec = t.sec;
            update_display_time(ledStrip, t.hour, t.min, t.sec);
        }
        //SEND TIME TO WIFI MODULE IF NEEDED
        if(last_tmin != t.min){
            last_tmin = t.min;
            wifi_interface::send_current_time(t.hour, t.min, t.sec);
        }

        //UPDATE BRIGHTNESS IF NEEDED
        if(current_brightness_mode == 0){
             current_brightness = get_average_brightness(); //automatic if mode = 0
        }else{
            current_brightness = current_brightness_mode; //manual if mode 1-255
        }
        
        if (current_brightness != last_brightness){
            last_brightness = current_brightness;
            wifi_interface::send_brightness(current_brightness);
            ledStrip.setBrightness(current_brightness);
        }

       
       //PARSE CMD FROM WIFIMOUDLE IF PRESENT
       wifi_interface::rxcmd wmc = wifi_interface::manual_uart_rx();
       if(wmc.cmdok){
            printf("got cmd %s %s \n", wmc.cmd.c_str(), wmc.payload.c_str());
            if(wmc.cmd == "st"){
                rtc::set_rtc_time(wmc.payload);
            }else if(wmc.cmd == "sb"){
                current_brightness_mode = std::max(0,std::min(255,std::atoi(wmc.payload.c_str())));
            }
       }

        sleep_ms(100);
    }

    return 0;
}
