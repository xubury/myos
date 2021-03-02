#include "BasicRenderer.hpp"
#include "Bitmap.hpp"
#include "BootInfo.h"
#include "EFIMemory.hpp"
#include "Memory.hpp"
#include "String.hpp"

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
    renderer.print(" MB\n");
    renderer.setColor(RGBA(255, 255, 255), RGBA());

    uint8_t buffer[20] = {0};
    Bitmap bitmap;
    bitmap.setBuffer(buffer, 20);
    bitmap.set(0, false);
    bitmap.set(1, true);
    bitmap.set(2, false);
    bitmap.set(3, false);
    bitmap.set(4, true);
    for (int i = 0; i < 20; ++i) {
        renderer.print(toHexString(buffer[i]));
        renderer.print(" ");
    }
}
