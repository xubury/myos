#include "BasicRenderer.hpp"
#include "BootInfo.h"
#include "EFIMemory.hpp"
#include "Memory.hpp"
#include "string.hpp"

extern "C" void _start(BootInfo *info) {
    BasicRenderer renderer(info->frameBuffer, info->psf1Font);
    renderer.clearScreen();
    renderer.print("This is the first line!\n");
    renderer.setColor(RGBA(0, 255, 255), RGBA());
    renderer.print("Welcome to my kernel!\n");
    renderer.print(toString(123.232));
    renderer.print(" ");
    renderer.print(toString(123));
    renderer.print(" ");
    renderer.print(toString(-1));
    renderer.print(" ");
    renderer.print(toString(0));
    renderer.print(" ");
    renderer.print(toHexString(255));
    renderer.print(" ");
    renderer.print(toHexString<float>(1.2));
    renderer.newLine();

    renderer.print("Total memeory size: ");
    renderer.setColor(RGBA(255, 0, 0), RGBA());
    uint64_t mapEntries = info->mapSize / info->mapDescriptorSize;
    renderer.print(toString(
        (float)getMemorySize(info->map, mapEntries, info->mapDescriptorSize) /
        (1024 * 1024)));
    renderer.print(" MB");
    renderer.setColor(RGBA(255, 255, 255), RGBA());
}
