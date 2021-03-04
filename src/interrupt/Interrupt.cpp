#include "Interrupt.hpp"
#include "Panic.hpp"

__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *) {
    panic("Page fault detected.\n");
    asm("hlt");
}
