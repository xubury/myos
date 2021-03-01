#include "BasicRenderer.hpp"
#include "BootInfo.h"
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
}
