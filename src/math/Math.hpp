#ifndef MATH_HPP
#define MATH_HPP

#include <stdint.h>

struct RGBA {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
    RGBA(uint32_t rgba);
    explicit operator uint32_t();
};

struct Cursor {
    uint32_t x;
    uint32_t y;
    RGBA foregoundColor;
    RGBA backgoundColor;
    Cursor(uint32_t x = 0, uint32_t y = 0, RGBA foregound = RGBA(255, 255, 255),
           RGBA background = RGBA(0, 0, 0));
};

#endif
