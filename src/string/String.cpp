#include "String.hpp"

char buffer[128];

char* toString(float value, uint8_t decimalPlaces) {
    return toString((double)value, decimalPlaces);
}

char* toString(double value, uint8_t decimalPlaces) {
    if (decimalPlaces > 20) decimalPlaces = 20;
    char* doublePtr = toString<int64_t>(value);

    if (value < 0) {
        value = -value;
    }

    while (*doublePtr != 0) {
        ++doublePtr;
    }

    *doublePtr = '.';
    ++doublePtr;

    double newValue = value - (int64_t)value;

    for (uint8_t i = 0; i < decimalPlaces; i++) {
        newValue *= 10;
        *doublePtr = (int8_t)newValue + '0';
        newValue -= (int64_t)newValue;
        doublePtr++;
    }

    *doublePtr = 0;
    return buffer;
}
