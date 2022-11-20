#include "helper.h"

int helper::map(const int x, const int in_min, const int in_max, const int out_min, const int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}