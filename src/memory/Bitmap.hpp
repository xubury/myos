#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <stddef.h>
#include <stdint.h>

class Bitmap {
   public:
    bool operator[](uint64_t index);
    void set(uint64_t index, bool value);

    uint8_t *buffer;
    size_t size;
};

#endif
