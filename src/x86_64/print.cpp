#include "print.hpp"

const size_t Printer::NUM_COLS = 80;
const size_t Printer::NUM_ROWS = 25;

Char *Printer::m_buffer = (Char *)0xb8000;
uint8_t Printer::m_color = PRINT_COLOR_WHITE + (PRINT_COLOR_BLACK << 4);
uint8_t Printer::m_row = 0;
uint8_t Printer::m_col = 0;

void Printer::clearRow(size_t row) {
    Char empty;
    for (size_t col = 0; col < NUM_COLS; ++col) {
        m_buffer[col + NUM_COLS * row] = empty;
    }
}

void Printer::printClear() {
    for (size_t row = 0; row < NUM_ROWS; row++) {
        clearRow(row);
    }
}

void Printer::printNewline() {
    m_col = 0;
    if (m_row < NUM_ROWS - 1) {
        ++m_row;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; ++row) {
        for (size_t col = 0; col < NUM_COLS; ++col) {
            struct Char character = m_buffer[col + NUM_COLS * row];
            m_buffer[col + NUM_ROWS * (row - 1)] = character;
        }
    }

    clearRow(NUM_ROWS - 1);
}

void Printer::printChar(char character) {
    if (character == '\n') {
        printNewline();
    }

    if (m_col > NUM_COLS) {
        printNewline();
    }

    m_buffer[m_col + NUM_COLS * m_row] = Char(character, m_color);

    ++m_col;
}

void Printer::printStr(char *str) {
    for (size_t i = 0; 1; ++i) {
        char character = (uint8_t)str[i];

        if (character == '\0') {
            return;
        }
        printChar(character);
    }
}

void Printer::printSetColor(uint8_t foreground, uint8_t background) {
    m_color = foreground + (background << 4);
}
