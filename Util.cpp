#include "Util.h"
#include <iostream>
#include <sstream>

int convertHexCharArrayToInt(char* array, bool isBigEndian, int len) {
    int result = 0;
    if (isBigEndian) {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * (len - 1 - i));
        }
    }
    else {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * i);
        }
    }
    return result;
}
