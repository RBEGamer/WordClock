#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <vector>
#include <cstring>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class helper
{
public:
static int map(const int x, const int in_min, const int in_max, const int out_min, const int out_max);
static void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out);
static std::vector<bool> bits_from_int(const int integer);
static int limit(const int _value, const int _min, const int _max);
static int limit(const std::string& _value, const int _min, const int _max);


static int reg_write(i2c_inst* _i2c, const uint _addr, const uint8_t _reg, uint8_t* _buf, const uint8_t _bytes);
static int reg_read(i2c_inst* _i2c, const uint _addr, const uint8_t _reg, uint8_t* _buf, const uint8_t _bytes);
static int version2int(const std::string _version);
};
#endif