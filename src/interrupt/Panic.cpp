#include "KernelUtil.hpp"
#include "Panic.hpp"

void panic(const char* panicMessage) {
    BasicRenderer& renderer = manager().renderer();
    renderer.clearScreen();
    renderer.setCurosr(0, 0);
    renderer.print("Kernel Panic:\n");
    renderer.print(panicMessage);
}
