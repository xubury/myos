#include "print.hpp"

extern "C" {

void kernel_main() {
    Printer::printClear();
    Printer::printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    const char *str = "Welcome to my kernel!";
    Printer::printStr(str);
}
}
