#include "PageFrameAllocator.hpp"

PageFrameAllocator g_PageFrameAllocator;

size_t PageFrameAllocator::freeMemory;
size_t PageFrameAllocator::reservedMemory;
size_t PageFrameAllocator::usedMemory;
bool PageFrameAllocator::initialized = false;

void PageFrameAllocator::readEFIMemoryMap(EFIMemoryDescriptor *map,
                                          size_t mapSize,
                                          size_t descriptorSize) {
    if (initialized) return;

    initialized = true;

    size_t mapEntries = mapSize / descriptorSize;

    void *maxFreeSegment = nullptr;
    size_t maxSize = 0;
    for (size_t i = 0; i < mapEntries; ++i) {
        EFIMemoryDescriptor *desc =
            (EFIMemoryDescriptor *)((uint64_t)map + (i * descriptorSize));
        if (desc->Type == EFIConventionalMemory) {
            if (desc->NumberOfPages * 4096 > maxSize) {
                maxFreeSegment = (void *)desc->PhysicalStart;
                maxSize = desc->NumberOfPages * 4096;
            }
        }
    }

    size_t memorySize = getMemorySize(map, mapSize, descriptorSize);
    freeMemory = memorySize;

    size_t bitmapSize = memorySize / 4096 / 8 + 1;
    initBitmap(maxFreeSegment, bitmapSize);

    // lock pages of bitmap
    // reserved pages of unusable/reserved memory
    lockPages(m_pageBitmap.buffer, m_pageBitmap.size / 4096 + 1);

    for (size_t i = 0; i < mapEntries; ++i) {
        EFIMemoryDescriptor *desc =
            (EFIMemoryDescriptor *)((uint64_t)map + (i * descriptorSize));
        if (desc->Type != EFIConventionalMemory) {
            reservePages((void *)desc->PhysicalStart, desc->NumberOfPages);
        }
    }
}

void *PageFrameAllocator::requestPage() {
    for (size_t i = 0; i < m_pageBitmap.size * 8; ++i) {
        if (m_pageBitmap[i]) continue;
        lockPage((void *)(i * 4096));
        return (void *)(i * 4096);
    }
    return nullptr;  // TODO:page frame swap
}

void PageFrameAllocator::initBitmap(void *bufferAddr, size_t bitmapSize) {
    m_pageBitmap.buffer = (uint8_t *)bufferAddr;
    m_pageBitmap.size = bitmapSize;

    for (size_t i = 0; i < bitmapSize; ++i) {
        *(m_pageBitmap.buffer + i) = 0;
    }
}

void PageFrameAllocator::freePage(void *addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (m_pageBitmap[index] == false) {
        return;
    }
    if (m_pageBitmap.set(index, false)) {
        freeMemory += 4096;
        usedMemory -= 4096;
    }
}

void PageFrameAllocator::lockPage(void *addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (m_pageBitmap[index] == true) {
        return;
    }
    if (m_pageBitmap.set(index, true)) {
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator::reservePage(void *addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (m_pageBitmap[index] == false) {
        return;
    }

    if (m_pageBitmap.set(index, false)) {
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

void PageFrameAllocator::unreservePage(void *addr) {
    uint64_t index = (uint64_t)addr / 4096;
    if (m_pageBitmap[index] == true) {
        return;
    }
    if (m_pageBitmap.set(index, true)) {
        freeMemory += 4096;
        reservedMemory -= 4096;
    }
}

void PageFrameAllocator::freePages(void *addr, size_t pageCnt) {
    for (size_t i = 0; i < pageCnt; ++i) {
        freePage((void *)((uint64_t)addr + (i * 4096)));
    }
}

void PageFrameAllocator::lockPages(void *addr, size_t pageCnt) {
    for (size_t i = 0; i < pageCnt; ++i) {
        lockPage((void *)((uint64_t)addr + (i * 4096)));
    }
}
void PageFrameAllocator::reservePages(void *addr, size_t pageCnt) {
    for (size_t i = 0; i < pageCnt; ++i) {
        reservePage((void *)((uint64_t)addr + (i * 4096)));
    }
}

void PageFrameAllocator::unreservePages(void *addr, size_t pageCnt) {
    for (size_t i = 0; i < pageCnt; ++i) {
        unreservePage((void *)((uint64_t)addr + (i * 4096)));
    }
}
size_t PageFrameAllocator::getFreeRAM() { return freeMemory; }

size_t PageFrameAllocator::getUsedRAM() { return usedMemory; }

size_t PageFrameAllocator::getReservedRAM() { return reservedMemory; }
