#ifndef STRING_HPP
#define STRING_HPP

#include <stddef.h>
#include <stdint.h>

extern char buffer[128];

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

char* toString(double value, uint8_t decimalPlaces = 2);

char* toString(float value, uint8_t decimalPlaces = 2);

template <typename T>
inline char* toHexString(T value) {
    uint8_t* valPtr = (uint8_t*)&value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t bytes = sizeof(decltype(value));
    buffer[0] = '0';
    buffer[1] = 'x';
    uint8_t size = bytes * 2 + 2;
    for (uint8_t i = 0; i < bytes; ++i) {
        ptr = (valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        buffer[size - 1 - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        buffer[size - 1 - i * 2] = tmp + (tmp > 9 ? 55 : '0');
    }
    buffer[size] = 0;
    return buffer;
}

#endif
