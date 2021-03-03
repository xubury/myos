#include "Interrupt.hpp"
#include "KernelUtil.hpp"

__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *) {
    g_manager.renderer().print("Page fault detected\n");
    while (true)
        ;
}
