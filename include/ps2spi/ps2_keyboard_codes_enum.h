#ifndef PS2_KEYBOARD_CODES_H
#define PS2_KEYBOARD_CODES_H

typedef enum
{
#define PS2_KEYBOARD_DEFINE_KEY(name0, type0, set1_code0, set2_code0, name1, type1, set1_code1, set2_code1) PS2_KEYBOARD_##name0, PS2_KEYBOARD_##name1,
#include "ps2_keyboard_codes.def"
#undef PS2_KEYBOARD_DEFINE_KEY
	PS2_KEYBOARD_KEY_COUNT
} Ps2KeyboardKeyCode;

#endif /* PS2_KEYBOARD_CODES_H */
