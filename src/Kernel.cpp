#include "KernelUtil.hpp"

extern "C" void _start(BootInfo *info) {
    g_manager.initKernel(info);

    BasicRenderer &renderer = g_manager.renderer();
    renderer.setColor(RGBA(0, 255, 255), RGBA());
    renderer.print("\nWelcome to my kernel!\n");
    renderer.setColor(RGBA(255, 255, 255), RGBA());

    g_manager.testString();
    g_manager.testBitmap();
    g_manager.testPageIndexer();

    g_manager.printMemory();

    asm("int $0x0e");
    while (true)
        ;
}
