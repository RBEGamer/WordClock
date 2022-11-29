#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <vector>
#include <cstring>


class helper
{
public:
static int map(const int x, const int in_min, const int in_max, const int out_min, const int out_max);
static void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out);
static std::vector<bool> bits_from_int(const int integer);
};
#endif