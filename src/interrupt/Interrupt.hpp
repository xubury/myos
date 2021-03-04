#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

struct interrupt_frame;
__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *frame);
__attribute__((interrupt)) void doubleFaultHandler(interrupt_frame *frame);
__attribute__((interrupt)) void gPFaultHandler(interrupt_frame *frame);
#endif
