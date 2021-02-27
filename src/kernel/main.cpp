#include "VGABuffer.hpp"
#include "gdt/GDT.hpp"

void initGDT() {
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT) - 1;
    gdtDescriptor.offset = (uint64_t)&defaultGDT;
    loadGDT(&gdtDescriptor);
}

extern "C" void kernelStart() {
    initGDT();
    VGA::printClear();
    VGA::printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    VGA::printStr("Welcome to my kernel!\n");
}
