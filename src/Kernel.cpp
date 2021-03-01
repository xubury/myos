#include "BasicRenderer.hpp"
#include "string.hpp"

extern "C" int _start(FrameBuffer *frameBuffer, PSF1Font *fontBuffer) {
    BasicRenderer renderer(frameBuffer, fontBuffer);
    renderer.clearScreen();
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
    return 1;
}
