#include "Math.hpp"

RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : a(a), r(r), g(g), b(b) {}

RGBA::RGBA(uint32_t rgba) {
    uint8_t* p = (uint8_t*)&rgba;
    a = p[3];
    r = p[2];
    g = p[1];
    b = p[0];
}

RGBA::operator uint32_t() {
    uint32_t rgba;
    uint8_t* p = (uint8_t*)&rgba;
    p[3] = a;
    p[2] = r;
    p[1] = g;
    p[0] = b;
    return rgba;
}

Cursor::Cursor(uint32_t x, uint32_t y, RGBA foregound, RGBA background)
    : x(x), y(y), foregoundColor(foregound), backgoundColor(background) {}
