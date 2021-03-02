#include "BasicRenderer.hpp"
#include "Bitmap.hpp"
#include "BootInfo.h"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"
#include "Paging.hpp"
#include "String.hpp"

extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

void printMemory(BasicRenderer &renderer) {
    renderer.print("Memory:\n");
    renderer.setColor(RGBA(255, 0, 0), RGBA());
    renderer.print("Free:");
    renderer.print(
        toString((float)g_PageFrameAllocator.getFreeRAM() / (1024 * 1024)));
    renderer.print(" MB\n");
    renderer.print("Used:");
    renderer.print(
        toString((float)g_PageFrameAllocator.getUsedRAM() / (1024 * 1024)));
    renderer.print(" MB\n");
    renderer.print("Reserved:");
    renderer.print(
        toString((float)g_PageFrameAllocator.getReservedRAM() / (1024 * 1024)));
    renderer.print(" MB\n");
    renderer.setColor(RGBA(255, 255, 255), RGBA());
}

void testString(BasicRenderer &renderer) {
    renderer.print("\nTesting string conversion:\n");
    renderer.print("Expecting: 123.23 123 -1 0 0x000000FF 0x3F99999A\n");
    renderer.print("Getting: ");
    renderer.print(toString(123.232));
    renderer.print(" ");
    renderer.print(toString(123));
    renderer.print(" ");
    renderer.print(toString(-1));
    renderer.print(" ");
    renderer.print(toString(0));
    renderer.print(" ");
    renderer.print(toHexString(255));  // 0xFF
    renderer.print(" ");
    renderer.print(toHexString<float>(1.2));  // 0x3F99999A
    renderer.newLine();
}

void testBitmap(BasicRenderer &renderer) {
    renderer.print("\nTesting bitmap:\n");
    renderer.print("Expecting: 0x48 0x00 .... 0x00\n");
    renderer.print("Getting: ");
    uint8_t buffer[20] = {0};
    Bitmap bitmap;
    bitmap.buffer = buffer;
    bitmap.set(0, false);
    bitmap.set(1, true);
    bitmap.set(2, false);
    bitmap.set(3, false);
    bitmap.set(4, true);
    for (int i = 0; i < 20; ++i) {
        renderer.print(toHexString(buffer[i]));
        renderer.print(" ");
    }
    renderer.newLine();
}

void testPageIndexer(BasicRenderer &renderer) {
    renderer.print("\nTesting page indexer:\n");
    renderer.print("Expecting: 0-0-0-1\n");
    renderer.print("Getting: ");
    PageMapIndexer pageIndexer = PageMapIndexer(0x1000);
    renderer.print(toString(pageIndexer.pDPIndex));
    renderer.print("-");
    renderer.print(toString(pageIndexer.pDIndex));
    renderer.print("-");
    renderer.print(toString(pageIndexer.pTIndex));
    renderer.print("-");
    renderer.print(toString(pageIndexer.pIndex));
    renderer.newLine();
}

void initKernal(BootInfo *info) {
    g_PageFrameAllocator.readEFIMemoryMap(info->map, info->mapSize,
                                          info->mapDescriptorSize);
    uint64_t kernelSize = (uint64_t)&_kernelEnd - (uint64_t)&_kernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    g_PageFrameAllocator.lockPages(&_kernelStart, kernelPages);
    PageTable *p4Table = (PageTable *)g_PageFrameAllocator.requestPage();

    memset(p4Table, 0, 0x1000);
    PageTableManager pageManager = PageTableManager(p4Table);
    size_t memorySize =
        getMemorySize(info->map, info->mapSize, info->mapDescriptorSize);
    for (uint64_t i = 0; i < memorySize; i += 0x1000) {
        pageManager.mapMemory((void *)i, (void *)i);
    }

    uint64_t fbBase = (uint64_t)info->frameBuffer->baseAddr;
    uint64_t fbSize = (uint64_t)info->frameBuffer->bufferSize + 0x1000;
    for (uint64_t i = fbBase; i < fbBase + fbSize; i += 0x1000) {
        pageManager.mapMemory((void *)i, (void *)i);
    }
    asm("mov %0, %%cr3" : : "r"(p4Table));
}

extern "C" void _start(BootInfo *info) {
    initKernal(info);
    BasicRenderer renderer(info->frameBuffer, info->psf1Font);
    renderer.clearScreen();
    renderer.setColor(RGBA(0, 255, 255), RGBA());
    renderer.print("Welcome to my kernel!\n");
    renderer.setColor(RGBA(255, 255, 255), RGBA());

    printMemory(renderer);
    testString(renderer);
    testBitmap(renderer);
    testPageIndexer(renderer);
}
