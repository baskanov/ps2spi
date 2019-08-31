#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ps2spi/ps2_keyboard_codes_enum.h>

enum
{
	KEYBOARD_MODIFIER_LEFT_CTRL = 0x01,
	KEYBOARD_MODIFIER_LEFT_ALT = 0x02,
	KEYBOARD_MODIFIER_LEFT_SHIFT = 0x04,
	KEYBOARD_MODIFIER_NUM_LOCK = 0x10,
	KEYBOARD_MODIFIER_RIGHT_CTRL = 0x20,
	KEYBOARD_MODIFIER_RIGHT_ALT = 0x40,
	KEYBOARD_MODIFIER_RIGHT_SHIFT = 0x88
};

typedef struct
{
	unsigned char clock_mask;
	unsigned char data_mask;
	unsigned char state;
	unsigned char scanning_enabled;
	unsigned char scancode_set;
	unsigned char data[16];
	unsigned char start;
	unsigned char size;
	unsigned char last_byte_sent;
	unsigned char option_byte;
	unsigned char modifiers;
	unsigned char leds;
	unsigned char typematic_delay;
	unsigned char typematic_period;
	Ps2KeyboardKeyCode last_typematic_key;
	unsigned char typematic_timer;
	unsigned char bat_timer;
	unsigned char reset_timer;
} Keyboard;

void keyboard_init(Keyboard *self, unsigned char clock_mask, unsigned char data_mask);
void keyboard_work(Keyboard *self, unsigned char time_delta);
void keyboard_set_key_state(Keyboard *self, Ps2KeyboardKeyCode key_code, unsigned char release);
unsigned char keyboard_get_leds(Keyboard *self);

#endif /* KEYBOARD_H */
