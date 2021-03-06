#include "Keyboard.hpp"

void handleKeyboard(uint8_t scanCode) {
    if (scanCode == QWERTYKeyboard::BackSpace) {
        manager().renderer().backSapce(1);
    } else if (scanCode == QWERTYKeyboard::Enter) {
        manager().renderer().newLine();
    } else {
        char ascii = QWERTYKeyboard::translate(scanCode, false);
        if (ascii != 0) {
            manager().renderer().putChar(ascii);
        }
    }
}
