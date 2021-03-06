#ifndef SCAN_CDOE_HPP
#define SCAN_CDOE_HPP

#include <stdint.h>

namespace ASCII {
extern const char table[];

}  // namespace ASCII

namespace QWERTYKeyboard {

const uint8_t LeftShift = 0x2A;
const uint8_t RightShift = 0x36;
const uint8_t Enter = 0x1C;
const uint8_t BackSpace = 0x0E;
const uint8_t Spacebar = 0x39;

char translate(uint8_t scanCode, bool upperCase);

}  // namespace QWERTYKeyboard

#endif
