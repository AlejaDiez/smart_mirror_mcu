#ifndef __UTILS
#define __UTILS

#include <Arduino.h>

inline int getChipId() {
    uint32_t chipId = 0;

    for (int i = 0; i < 17; i += 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    return chipId;
}

#endif