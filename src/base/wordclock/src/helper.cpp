#include "helper.h"

int helper::map(const int x, const int in_min, const int in_max, const int out_min, const int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void helper::tokenize(std::string const &str, const char *delim, std::vector<std::string> &out)
{
  char *token = strtok(const_cast<char *>(str.c_str()), delim);
  while (token != nullptr)
  {
    out.push_back(std::string(token));
    token = strtok(nullptr, delim);
  }
}

int helper::version2int(const std::string _version)
{
  const char *delim = ".";
  std::vector<std::string> out;
  helper::tokenize(_version, delim, out);
  if (out.size() > 2)
  {
    return std::atoi((out.at(0) + out.at(1) + out.at(2)).c_str());
  }
  return 0;
}

std::vector<bool> helper::bits_from_int(const int integer) // discern which bits of PLC codes are true
{
  std::vector<bool> bool_bits;
  int tmp = integer;
  // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
  for (int i = 0; i < 8; i++)
  {
    bool_bits.push_back(tmp % 2); // remainder of dividing by 2
    tmp /= 2;                     // integer equals itself divided by 2
  }

  return bool_bits;
}

int helper::limit(const int _value, const int _min, const int _max)
{
  return std::max(_min, std::min(_max, _value));
}

int helper::limit(const std::string &_value, const int _min, const int _max)
{
  return helper::limit(std::atoi(_value.c_str()), _min, _max);
}

int helper::reg_write(i2c_inst *_i2c, const uint _addr, const uint8_t _reg, uint8_t *_buf, const uint8_t _bytes)
{
  int num_bytes_read = 0;
  uint8_t msg[_bytes + 1];

  // Check to make sure caller is sending 1 or more bytes
  if (_bytes < 1)
  {
    return 0;
  }

  // Append register address to front of data packet
  msg[0] = _reg;
  for (int i = 0; i < _bytes; i++)
  {
    msg[i + 1] = _buf[i];
  }

  // Write data to register(s) over I2C
  const int ret = i2c_write_blocking(_i2c, _addr, msg, (_bytes + 1), false);

  // printf("write %i %i\n", msg[1], ret);
  return num_bytes_read;
}

int helper::reg_read(i2c_inst *_i2c, const uint _addr, const uint8_t _reg, uint8_t *_buf, const uint8_t _bytes)
{
  int num_bytes_read = 0;

  // Check to make sure caller is asking for 1 or more bytes
  if (_bytes < 1)
  {
    return 0;
  }

  // Read data from register(s) over I2C
  i2c_write_blocking(_i2c, _addr, &_reg, 1, true);
  num_bytes_read = i2c_read_blocking(_i2c, _addr, _buf, _bytes, false);
  // printf("read %i %i\n", _buf[0], num_bytes_read);
  return num_bytes_read;
}