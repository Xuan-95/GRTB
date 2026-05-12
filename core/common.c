#include "common.h"

unsigned char floatToByte(float value) {
    if (value <= 0.0)
        return 0;
    else if (value >= 1.0)
        return 255;
    return (unsigned char)(value * 256.0);
}

int clamp(int x, int low, int high) {
    if (x <= low)
        return low;
    else if (x >= high)
        return high;
    return x;
}
