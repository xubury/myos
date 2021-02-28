#include "VGABuffer.hpp"

extern "C" int _start(FrameBuffer *frameBuffer, PSF1Font *fontBuffer) {
    VGABuffer vga(frameBuffer, fontBuffer);
    vga.clearScreen();
    vga.print("Welcome to my kernel!");
    return 123;
}
