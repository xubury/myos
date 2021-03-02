#include "Bitmap.hpp"

bool Bitmap::operator[](uint64_t index) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    // big endian
    uint8_t bitindexer = 0b10000000 >> bitIndex;
    if ((m_buffer[byteIndex] & bitindexer) > 0)
        return true;
    else
        return false;
}

void Bitmap::set(uint64_t index, bool value) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    // big endian
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    m_buffer[byteIndex] &= ~bitIndexer;
    if (value) {
        m_buffer[byteIndex] |= bitIndexer;
    }
}

size_t Bitmap::size() { return m_size; }

void Bitmap::setBuffer(uint8_t *ptr, size_t size) {
    m_buffer = ptr;
    m_size = size;
}
