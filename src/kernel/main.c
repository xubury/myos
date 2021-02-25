#include "print.h"

void kernel_main() {
    printClear();
    printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    printStr("Welcome to my kernel!");
}
