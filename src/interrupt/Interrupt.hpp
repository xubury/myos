#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct InterruptFrame;
__attribute__((interrupt)) void pageFaultHandler(InterruptFrame *frame);
__attribute__((interrupt)) void doubleFaultHandler(InterruptFrame *frame);
__attribute__((interrupt)) void gPFaultHandler(InterruptFrame *frame);
__attribute__((interrupt)) void keyboardHandler(InterruptFrame *frame);

void remapPIC(uint8_t offset1, uint8_t offset2);

void picEndMaster();

void picEndSlave();

#endif
