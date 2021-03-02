#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <stddef.h>
#include <stdint.h>

class Bitmap {
   public:
    bool operator[](uint64_t index);
    void set(uint64_t index, bool value);

    size_t size();
    void setBuffer(uint8_t *ptr, size_t size);

   private:
    uint8_t *m_buffer;
    size_t m_size;
};

#endif
