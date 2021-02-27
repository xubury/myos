#include "VGABuffer.hpp"

extern "C" {

void kernel_main() {
    VGA::printClear();
    VGA::printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    VGA::printStr("Welcome to my kernel!\n");
    VGA::printSetColor(PRINT_COLOR_LIGHT_BLUE, PRINT_COLOR_BLACK);
    VGA::printStr("Test different color.\n");
    VGA::printSetColor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    const char *str =
        "Test long string\n"
        "line1..\n"
        "line2..\n"
        "line3..\n"
        "line4..\n"
        "line5..\n"
        "line6..\n"
        "line7..\n"
        "line8..\n"
        "line9..\n"
        "line10..\n"
        "line11..\n"
        "line12..\n"
        "line13..\n"
        "line14..\n"
        "line15..\n"
        "line16..\n"
        "line17..\n"
        "line18..\n"
        "line19..\n"
        "line20..\n"
        "line21..\n"
        "line22..\n"
        "line23..\n"
        "line24..\n"
        "line25..\n"
        "abcdefghijklmnopqrstuvwxyz0123456789+-*/"
        "abcdefghijklmnopqrstuvwxyz0123456789+-*/"
        "abcdefghijklmnopqrstuvwxyz0123456789+-*/";
    VGA::printStr(str);
}
}
