#ifndef PRINT_HPP
#define PRINT_HPP

#include <stddef.h>
#include <stdint.h>

enum {
    PRINT_COLOR_BLACK = 0,
    PRINT_COLOR_BLUE = 1,
    PRINT_COLOR_GREEN = 2,
    PRINT_COLOR_CYAN = 3,
    PRINT_COLOR_RED = 4,
    PRINT_COLOR_MAGENTA = 5,
    PRINT_COLOR_BROWN = 6,
    PRINT_COLOR_LIGHT_GRAY = 7,
    PRINT_COLOR_DARK_GRAY = 8,
    PRINT_COLOR_LIGHT_BLUE = 9,
    PRINT_COLOR_LIGHT_GREEN = 10,
    PRINT_COLOR_LIGHT_CYAN = 11,
    PRINT_COLOR_LIGHT_RED = 12,
    PRINT_COLOR_PINK = 13,
    PRINT_COLOR_YELLOW = 14,
    PRINT_COLOR_WHITE = 15,
};

struct Char {
    uint8_t character;
    uint8_t color;
    Char(uint8_t character = ' ',
         uint8_t color = PRINT_COLOR_WHITE + (PRINT_COLOR_BLACK << 4))
        : character(character), color(color) {}
};

class Printer {
   public:
    static void printClear();
    static void printChar(char character);
    static void printStr(const char *str);
    static void printSetColor(uint8_t foreground, uint8_t background);

   private:
    static void clearRow(size_t row);
    static void printNewline();
    static Char *m_buffer;
    static uint8_t m_color;
    static uint8_t m_row;
    static uint8_t m_col;

    const static size_t NUM_COLS;
    const static size_t NUM_ROWS;
};

#endif
