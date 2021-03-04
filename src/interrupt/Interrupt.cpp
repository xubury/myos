#include "Interrupt.hpp"
#include "Panic.hpp"

__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *) {
    panic("Page fault detected.\n");
    asm("hlt");
}

__attribute__((interrupt)) void doubleFaultHandler(interrupt_frame *) {
    panic("Double fault detected.\n");
    asm("hlt");
}

__attribute__((interrupt)) void gPFaultHandler(interrupt_frame *) {
    panic("General protection fault detected.\n");
    asm("hlt");
}
