#include "BasicRenderer.hpp"

extern "C" int _start(FrameBuffer *frameBuffer, PSF1Font *fontBuffer) {
    BasicRenderer renderer(frameBuffer, fontBuffer);
    renderer.clearScreen();
    renderer.setColor(RGBA(0, 255, 255), RGBA());
    renderer.print("Welcome to my kernel!");
    return 1;
}
