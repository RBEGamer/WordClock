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