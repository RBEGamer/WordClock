#include "ambient_light_bh1750.hpp"




ambient_light_bh1750::ambient_light_bh1750(){

}
ambient_light_bh1750::~ambient_light_bh1750(){

}

void ambient_light_bh1750::init(){
    // INIT SEQUENCE POWER DOWN, POWER UP, RESET, CONT READING MODE
    uint8_t buf[1];
    buf[0] = 0x00;
    i2c_write_blocking(i2c_default, BH1750_I2C_ADDR, buf, 1, false);
    buf[0] = 0x01;
    i2c_write_blocking(i2c_default, BH1750_I2C_ADDR, buf, 1, false);
    buf[0] = 0x07;
    i2c_write_blocking(i2c_default, BH1750_I2C_ADDR, buf, 1, false);
    buf[0] = 0x13;
    i2c_write_blocking(i2c_default, BH1750_I2C_ADDR, buf, 1, false);   
}


int ambient_light_bh1750::get_brightness()
{ 
    uint8_t buf[2];
    const int ret = i2c_read_blocking(i2c_default, BH1750_I2C_ADDR, buf, 2, false);
    if (ret <= 0)
    {
        return -1;
    }
    const int lux = ((buf[0] << 8) + buf[1]) / (1.2 * 2); // custom 1.2 factor added
    return lux;
}

int ambient_light_bh1750::get_average_brightness()
{

    long average;
    // subtract the last reading:
    avg_sum = avg_sum - ambient_light_bh1750::readings[reading_index];
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
    if (reading_index >= NUM_READINGS)
    {
        reading_index = 0;
    }
    average = avg_sum / NUM_READINGS;
    // 0-420lux => 0-255 led brightness and limit range
    return helper::limit(helper::map((int)average, BH1750_RAW_OUTPUT_MIN, BH1750_RAW_OUTPUT_MAX, WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN, WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX), WORDCLOCK_BRIGHTNESS_MODE_AUTO_MIN, WORDCLOCK_BRIGHTNESS_MODE_AUTO_MAX);

}


