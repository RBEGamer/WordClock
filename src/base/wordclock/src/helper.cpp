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


std::vector<bool> helper::bits_from_int(const int integer)    // discern which bits of PLC codes are true
{
    std::vector <bool> bool_bits;
    int tmp = integer;
    // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
    for (int i = 0; i < 8; i++)
    {
        bool_bits.push_back (tmp%2);    // remainder of dividing by 2
        tmp /= 2;    // integer equals itself divided by 2
    }

    return bool_bits;
}
