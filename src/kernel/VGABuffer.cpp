#include "VGABuffer.hpp"

const size_t VGA::NUM_COLS = 80;
const size_t VGA::NUM_ROWS = 25;

Char *VGA::m_buffer = (Char *)0xb8000;
uint8_t VGA::m_color = PRINT_COLOR_WHITE + (PRINT_COLOR_BLACK << 4);
size_t VGA::m_row = 0;
size_t VGA::m_col = 0;

void VGA::clearRow(size_t row) {
    Char empty;
    for (size_t col = 0; col < NUM_COLS; ++col) {
        m_buffer[col + NUM_COLS * row] = empty;
    }
}

void VGA::printClear() {
    for (size_t row = 0; row < NUM_ROWS; row++) {
        clearRow(row);
    }
}

void VGA::printNewline() {
    m_col = 0;
    if (m_row < NUM_ROWS - 1) {
        ++m_row;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; ++row) {
        for (size_t col = 0; col < NUM_COLS; ++col) {
            Char character = m_buffer[col + NUM_COLS * row];
            m_buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clearRow(NUM_ROWS - 1);
}

void VGA::printChar(char character) {
    if (character == '\n') {
        printNewline();
    } else {
        if (m_col >= NUM_COLS) {
            printNewline();
        }
        m_buffer[m_col + NUM_COLS * m_row] = Char(character, m_color);
        ++m_col;
    }
}

void VGA::printStr(const char *str) {
    for (size_t i = 0; 1; ++i) {
        char character = str[i];

        if (character == '\0') {
            return;
        }
        printChar(character);
    }
}

void VGA::printSetColor(uint8_t foreground, uint8_t background) {
    m_color = foreground + (background << 4);
}
