#include "IO.hpp"
#include "Interrupt.hpp"
#include "KernelUtil.hpp"
#include "Keyboard.hpp"
#include "Panic.hpp"

__attribute__((interrupt)) void pageFaultHandler(InterruptFrame *) {
    manager().renderer().print("Page fault detected.\n");
}

__attribute__((interrupt)) void doubleFaultHandler(InterruptFrame *) {
    manager().renderer().print("Double fault detected.\n");
}

__attribute__((interrupt)) void gPFaultHandler(InterruptFrame *) {
    panic("General protection fault detected.\n");
    asm("hlt");
}

__attribute__((interrupt)) void keyboardHandler(InterruptFrame *) {
    uint8_t scanCode = inb(0x60);
    handleKeyboard(scanCode);
    picEndMaster();
}

void remapPIC(uint8_t offset1, uint8_t offset2) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait();

    outb(PIC1_DATA, offset1);
    ioWait();
    outb(PIC2_DATA, offset2);
    ioWait();

    outb(PIC1_DATA, 4);
    ioWait();
    outb(PIC2_DATA, 2);
    ioWait();

    outb(PIC1_DATA, ICW4_8086);
    ioWait();
    outb(PIC2_DATA, ICW4_8086);
    ioWait();

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void picEndMaster() { outb(PIC1_COMMAND, PIC_EOI); }

void picEndSlave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
