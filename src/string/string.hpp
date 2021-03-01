#ifndef STRING_HPP
#define STRING_HPP

#include <stddef.h>
#include <stdint.h>

inline char buffer[128];

template <typename T>
inline char* toString(T value) {
    uint8_t size = 0;

    if (value < 0) {
        ++size;
        buffer[0] = '-';
        value = -value;
    }

    uint64_t sizeTest = value;
    do {
        sizeTest /= 10;
        ++size;
    } while (sizeTest > 0);

    uint8_t index = 0;
    do {
        uint8_t remainder = value % 10;
        value /= 10;
        buffer[size - 1 - index] = remainder + '0';
        ++index;
    } while (value > 0);

    buffer[size] = 0;
    return buffer;
}

inline char* toString(double value, uint8_t decimalPlaces = 2) {
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

template <typename T>
inline char* toHexString(T value) {
    char* valPtr = (char*)&value;
    char* ptr;
    uint8_t tmp;
    uint8_t size = sizeof(decltype(value)) * 2 - 1;
    for (uint8_t i = 0; i < size; ++i) {
        ptr = (valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        buffer[size - i * 2] = tmp + (tmp > 9 ? 55 : '0');
    }
    buffer[size + 1] = 0;
    return buffer;
}

#endif
