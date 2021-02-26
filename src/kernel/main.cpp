#include "print.hpp"

extern "C" {

void kernel_main() {
    Printer::printClear();
    Printer::printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    Printer::printStr("Welcome to my kernel!");
}
}
