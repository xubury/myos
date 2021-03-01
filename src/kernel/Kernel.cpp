#include "BasicRenderer.hpp"

extern "C" int _start(FrameBuffer *frameBuffer, PSF1Font *fontBuffer) {
    BasicRenderer renderer(frameBuffer, fontBuffer);
    renderer.clearScreen();
    renderer.print("Welcome to my kernel!");
    return 123;
}
