#ifndef PAGE_FRAME_ALLOCATOR_HPP
#define PAGE_FRAME_ALLOCATOR_HPP

#include "Bitmap.hpp"
#include "Memory.hpp"

class PageFrameAllocator {
   public:
    void readEFIMemoryMap(EFIMemoryDescriptor *map, size_t mapSize,
                          size_t descriptorSize);
    void *requestPage();

    void freePage(void *addr);
    void lockPage(void *addr);

    void freePages(void *addr, size_t pageCnt);
    void lockPages(void *addr, size_t pageCnt);

    size_t getFreeRAM();
    size_t getUsedRAM();
    size_t getReservedRAM();

   private:
    void initBitmap(void *bufferAddr, size_t bitmapSize);

    void reservePage(void *addr);
    void unreservePage(void *addr);

    void reservePages(void *addr, size_t pageCnt);
    void unreservePages(void *addr, size_t pageCnt);

    Bitmap m_pageBitmap;
    size_t freeMemory;
    size_t reservedMemory;
    size_t usedMemory;
    bool initialized;
    size_t pageBitmapIndex;
};

#endif
