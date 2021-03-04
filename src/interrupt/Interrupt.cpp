#include "Interrupt.hpp"
#include "KernelUtil.hpp"

__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *) {
    manager().renderer().print("Page fault detected\n");
    asm("hlt");
}
