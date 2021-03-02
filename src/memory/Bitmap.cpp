#include "Bitmap.hpp"

bool Bitmap::operator[](uint64_t index) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    // big endian
    uint8_t bitindexer = 0b10000000 >> bitIndex;
    if ((buffer[byteIndex] & bitindexer) > 0)
        return true;
    else
        return false;
}

void Bitmap::set(uint64_t index, bool value) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    // big endian
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    buffer[byteIndex] &= ~bitIndexer;
    if (value) {
        buffer[byteIndex] |= bitIndexer;
    }
}
