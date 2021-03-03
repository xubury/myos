#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

struct interrupt_frame;
__attribute__((interrupt)) void pageFaultHandler(interrupt_frame *frame);
#endif
