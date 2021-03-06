#include "KernelUtil.hpp"
#include "Keyboard.hpp"

bool isLeftShiftPressed;
bool isRightShiftPressed;
void handleKeyboard(uint8_t scanCode) {
    switch (scanCode) {
        case QWERTYKeyboard::BackSpace:
            manager().renderer().backSapce(1);
            break;
        case QWERTYKeyboard::Enter:
            manager().renderer().newLine();
            break;
        case QWERTYKeyboard::LeftShift:
            isLeftShiftPressed = true;
            break;
        case QWERTYKeyboard::RightShift:
            isLeftShiftPressed = true;
            break;
        case QWERTYKeyboard::LeftShift + 0x80:
            isLeftShiftPressed = false;
            break;
        case QWERTYKeyboard::RightShift + 0x80:
            isLeftShiftPressed = false;
            break;
        default:
            char ascii = QWERTYKeyboard::translate(
                scanCode, isLeftShiftPressed || isRightShiftPressed);
            if (ascii != 0) {
                manager().renderer().putChar(ascii);
            }
            break;
    }
}
