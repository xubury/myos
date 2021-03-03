#include "KernelUtil.hpp"

extern "C" void _start(BootInfo *info) {
    KernelManager manager(info);

    manager.testString();
    manager.testBitmap();
    manager.testPageIndexer();

    manager.printMemory();

    BasicRenderer &renderer = manager.renderer();
    renderer.setColor(RGBA(0, 255, 255), RGBA());
    renderer.print("\nWelcome to my kernel!\n");
    renderer.setColor(RGBA(255, 255, 255), RGBA());

    while (true)
        ;
}
