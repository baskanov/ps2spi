#include <keyboard/keyboard.h>

#include <ps2/ps2.h>

#include <stddef.h>
#include <avr/pgmspace.h>

enum
{
#define DEFINE_STATE(name, next_state, parameter, procedure_offset, receive) name,
#include "keyboard_states.def"
#undef DEFINE_STATE
	STATE_COUNT
};

enum
{
#define DEFINE_KEY_TYPE(name, modifier, m0, m1, m2, m3, m4, m5, m6, m7, set1_selector, set2_selector) name,
#include "keyboard_key_types.def"
#undef DEFINE_KEY_TYPE
	KEY_TYPE_COUNT
};

#define MS_TO_TICKS(ms) (((ms) + 2u) / 4u)

static void clear(Keyboard *const self)
{
	self->size = 0;
}

static unsigned char empty(const Keyboard *const self)
{
	return 0 == self->size;
}

static unsigned char get_free_space(const Keyboard *const self)
{
	return 16 - self->size;
}

static unsigned char front(const Keyboard *const self)
{
	return self->data[self->start];
}

static void push_back(Keyboard *const self, const unsigned char byte)
{
	self->data[(self->start + self->size) % 16u] = byte;
	++self->size;
}

static void pop_front(Keyboard *const self)
{
	self->start = (self->start + 1) % 16u;
	--self->size;
}

static void set_default(Keyboard *const self)
{
	clear(self);
	self->typematic_delay = MS_TO_TICKS(500);
	self->typematic_period = MS_TO_TICKS(92);
	self->last_typematic_key = PS2_KEYBOARD_KEY_RESERVED;
}

static void reset(Keyboard *const self)
{
	self->state = STATE_SEND_BAT_SUCCESS;
	self->scanning_enabled = 1;
	self->scancode_set = 2;
	self->start = 0;
	self->last_byte_sent = 0;
	self->option_byte = 0;
	self->modifiers = 0;
	self->leds = 0;
	self->bat_timer = MS_TO_TICKS(1000);
	set_default(self);
}

void keyboard_init(Keyboard *const self, const unsigned char clock_mask, const unsigned char data_mask)
{
	self->clock_mask = clock_mask;
	self->data_mask = data_mask;
	reset(self);
}

static const unsigned char scancodes[][2] PROGMEM = {
#define PS2_KEYBOARD_DEFINE_KEY(name0, type0, set1_code0, set2_code0, name1, type1, set1_code1, set2_code1)            \
	(set1_code0), (set2_code0), (set1_code1), (set2_code1),
#include <ps2spi/ps2_keyboard_codes.def>
#undef PS2_KEYBOARD_DEFINE_KEY
};

/* clang-format off */

#define PACK_44(a, b) ((a) | ((b) << 4) | ((((a) > 0xf) || ((b) > 0xf)) ? 0x100 : 0))
#define UNPACK_44_0(a) ((unsigned char)((a) & 0xf))
#define UNPACK_44_1(a) ((unsigned char)((a) >> 4))

#define PACK_71(a, b) ((a) | ((b) << 7) | ((((a) > 0x7f) || ((b) > 0x1)) ? 0x100 : 0))
#define UNPACK_71_0(a) ((unsigned char)((a) & 0x7f))
#define UNPACK_71_1(a) ((unsigned char)((a) >> 7))

#define PACK_323(a, b, c) ((a) | ((b) << 3) | ((c) << 5) | ((((a) > 0x7) || ((b) > 0x3) || ((c) > 0x7)) ? 0x100 : 0))
#define UNPACK_323_0(a) ((unsigned char)((a) & 0x7))
#define UNPACK_323_1(a) ((unsigned char)(((a) >> 3) & 0x3))
#define UNPACK_323_2(a) ((unsigned char)((a) >> 5))

#define PACK_BITS(a, b, c, d, e, f, g, h) ((a) | ((b) << 1) | ((c) << 2) | ((d) << 3) | ((e) << 4) | ((f) << 5) | ((g) << 6) | ((h) << 7))

/* clang-format on */

static const unsigned char types[] PROGMEM = {
#define PS2_KEYBOARD_DEFINE_KEY(name0, type0, set1_code0, set2_code0, name1, type1, set1_code1, set2_code1)            \
	PACK_44((type0), (type1)),
#include <ps2spi/ps2_keyboard_codes.def>
#undef PS2_KEYBOARD_DEFINE_KEY
};

enum
{
#define DEFINE_SEQUENCE(name, rep, prefix, suffix) name##_prefix_length = prefix, name##_suffix_length = suffix,
#define X(byte) +1
#define SEQ_NIL +0
#define KEY_CODE +1
#include "keyboard_sequences.def"
#undef KEY_CODE
#undef SEQ_NIL
#undef X
#undef DEFINE_SEQUENCE
	SEQUENCE_END_prefix_length
};

enum
{
#define DEFINE_SEQUENCE(name, rep, prefix, suffix) name##_length = 1 prefix suffix,
#define X(byte) +1
#define SEQ_NIL +0
#define KEY_CODE +0
#include "keyboard_sequences.def"
#undef KEY_CODE
#undef SEQ_NIL
#undef X
#undef DEFINE_SEQUENCE
	SEQUENCE_END_length
};

static const struct Sequences
{
#define DEFINE_SEQUENCE(name, rep, prefix, suffix) unsigned char name##_sequence[name##_length];
#include "keyboard_sequences.def"
#undef DEFINE_SEQUENCE
} sequences PROGMEM = {
#define DEFINE_SEQUENCE(name, rep, prefix, suffix)                                                                     \
	{ PACK_323(name##_prefix_length, (rep), name##_suffix_length), prefix suffix },
#define X(byte) (0x##byte),
#define SEQ_NIL
#define KEY_CODE
#include "keyboard_sequences.def"
#undef KEY_CODE
#undef SEQ_NIL
#undef X
#undef DEFINE_SEQUENCE
};

enum
{
#define DEFINE_SEQUENCE(name, rep, prefix, suffix) name = offsetof(struct Sequences, name##_sequence),
#include "keyboard_sequences.def"
#undef DEFINE_SEQUENCE
	SEQUENCE_END
};

static const struct Selectors
{
#define DEFINE_SELECTOR(name, seqs) unsigned char name##_selector[seqs];
#define X(seq) +1
#include "keyboard_selectors.def"
#undef X
#undef DEFINE_SELECTOR
} selectors PROGMEM = {
#define DEFINE_SELECTOR(name, seqs) { seqs },
#define X(seq) (seq),
#include "keyboard_selectors.def"
#undef X
#undef DEFINE_SELECTOR
};

enum
{
#define DEFINE_SELECTOR(name, seqs) name = offsetof(struct Selectors, name##_selector),
#include "keyboard_selectors.def"
#undef DEFINE_SELECTOR
	SELECTOR_END
};

static const unsigned char key_type_modifiers[] = {
#define DEFINE_KEY_TYPE(name, modifier, m0, m1, m2, m3, m4, m5, m6, m7, set1_selector, set2_selector) (modifier),
#include "keyboard_key_types.def"
#undef DEFINE_KEY_TYPE
};

static const unsigned char key_type_masks[] = {
#define DEFINE_KEY_TYPE(name, modifier, m0, m1, m2, m3, m4, m5, m6, m7, set1_selector, set2_selector)                  \
	PACK_BITS((m0), (m1), (m2), (m3), (m4), (m5), (m6), (m7)),
#include "keyboard_key_types.def"
#undef DEFINE_KEY_TYPE
};

static const unsigned char key_type_selectors[][2] = {
#define DEFINE_KEY_TYPE(name, modifier, m0, m1, m2, m3, m4, m5, m6, m7, set1_selector, set2_selector) (set1_selector), (set2_selector),
#include "keyboard_key_types.def"
#undef DEFINE_KEY_TYPE
};

static void insert_scancode(
	Keyboard *const self, const Ps2KeyboardKeyCode key_code, const unsigned char release, const unsigned char repeat)
{
	const unsigned char scancode = pgm_read_byte_near((unsigned short)scancodes + key_code * 2u + self->scancode_set - 1u);
	const unsigned char type_pair = pgm_read_byte_near((unsigned short)types + key_code / 2u);
	const unsigned char type = (key_code % 2u) ? UNPACK_44_1(type_pair) : UNPACK_44_0(type_pair);
	const unsigned char mask = key_type_masks[type];
	const unsigned char modifier_mask = key_type_modifiers[type];
	const unsigned char bits = ((release | ((self->modifiers | (self->modifiers >> 5)) << 1)) & mask & 0xf) |
		((self->modifiers >> 1) & (mask >> 4));
	const unsigned char selector = key_type_selectors[type][self->scancode_set - 1u];
	const unsigned char offset = pgm_read_byte_near((unsigned short)&selectors + selector + bits);
	const unsigned char lengths = pgm_read_byte_near((unsigned short)&sequences + offset);
	const unsigned char prefix_length = UNPACK_323_0(lengths);
	const unsigned char repeat_length = UNPACK_323_1(lengths);
	const unsigned char suffix_length = UNPACK_323_2(lengths);
	const unsigned char total_length = prefix_length + suffix_length;
	const unsigned char free_space = get_free_space(self);
	if (free_space > total_length)
	{
		unsigned short address = (unsigned short)&sequences + offset;
		unsigned char i = total_length;
		if (repeat)
		{
			address += total_length - repeat_length;
			i = repeat_length;
		}
		for (; i > 0; --i)
		{
			unsigned char byte;
			if (suffix_length == i && scancode)
			{
				byte = scancode;
				if (release && self->scancode_set == 1)
					byte |= 0x80;
			}
			else
			{
				++address;
				byte = pgm_read_byte_near(address);
			}
			push_back(self, byte);
		}
	}
	else if (free_space >= 1)
	{
		push_back(self, self->scancode_set == 1 ? 0xff : 0x00);
	}
	if (!repeat)
	{
		self->modifiers &= ~modifier_mask | KEYBOARD_MODIFIER_NUM_LOCK;
		if (!release)
			self->modifiers ^= modifier_mask;

		if (!release || self->last_typematic_key == key_code)
		{
			self->last_typematic_key = repeat_length ? key_code : PS2_KEYBOARD_KEY_RESERVED;
			self->typematic_timer = self->typematic_delay;
		}
	}
}

static unsigned char send_byte_no_store_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!ps2_can_send(self->clock_mask, self->data_mask))
		return 0;

	if (!ps2_send(self->clock_mask, self->data_mask, parameter))
		return 0;

	return 1;
}

static unsigned char send_byte_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!send_byte_no_store_proc(self, parameter))
		return 0;

	self->last_byte_sent = parameter;

	return 1;
}

static unsigned char enable_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;

	clear(self);
	self->last_typematic_key = PS2_KEYBOARD_KEY_RESERVED;
	self->scanning_enabled = 1;

	return 1;
}

static unsigned char set_default_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;

	self->scanning_enabled &= parameter;
	set_default(self);

	return 1;
}

static unsigned char send_buffer_proc(Keyboard *const self, const unsigned char parameter)
{
	if (self->scanning_enabled)
	{
		if (!empty(self))
		{
			if (send_byte_proc(self, front(self)))
				pop_front(self);
		}
	}
	return 1;
}

static unsigned char set_leds_proc(Keyboard *const self, const unsigned char parameter)
{
	self->leds = self->option_byte;
	self->modifiers &= ~KEYBOARD_MODIFIER_NUM_LOCK;
	if (self->option_byte & 0x2)
		self->modifiers |= KEYBOARD_MODIFIER_NUM_LOCK;

	return 1;
}

static unsigned char set_scancode_set_proc(Keyboard *const self, const unsigned char parameter)
{
	if (0 == self->option_byte)
	{
		self->state = STATE_SET_SCANCODE_SET_SEND_CURRENT;
		return 0;
	}
	else if (self->option_byte <= 1)
	{
		clear(self);
		self->scancode_set = self->option_byte;
	}
	else
	{
		self->state = STATE_INVALID_COMMAND_0;
		return 0;
	}

	return 1;
}

static unsigned char send_scancode_set_proc(Keyboard *const self, const unsigned char parameter)
{
	return send_byte_proc(self, self->scancode_set);
}

static unsigned char set_typematic_rate_delay_proc(Keyboard *const self, const unsigned char parameter)
{
	self->typematic_delay = 250 / 4;
	if (self->option_byte & (1 << 5))
		self->typematic_delay += 250 / 4;
	if (self->option_byte & (1 << 6))
		self->typematic_delay += 250 / 4 * 2;
	self->typematic_period = (8 + (self->option_byte & 0x7));
	if (self->option_byte & 0x8)
		self->typematic_period <<= 1;
	if (self->option_byte & 0x10)
		self->typematic_period <<= 2;
	return 1;
}

static unsigned char resend_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!send_byte_no_store_proc(self, self->last_byte_sent))
		return 0;

	return 1;
}

static unsigned char reset_proc(Keyboard *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;

	self->reset_timer = MS_TO_TICKS(500);

	return 1;
}

static unsigned char reset_wait_proc(Keyboard *const self, const unsigned char parameter)
{
	if (0 == self->reset_timer)
		reset(self);
	return 0;
}

static unsigned char receive_option_byte_proc(Keyboard *const self, const unsigned char parameter)
{
	return 1;
}

#define PROCEDURES                                                                                                     \
	X(send_byte_proc)                                                                                                  \
	X(send_byte_no_store_proc)                                                                                         \
	X(enable_proc)                                                                                                     \
	X(set_default_proc)                                                                                                \
	X(resend_proc)                                                                                                     \
	X(send_buffer_proc)                                                                                                \
	X(set_leds_proc)                                                                                                   \
	X(set_scancode_set_proc)                                                                                           \
	X(set_typematic_rate_delay_proc)                                                                                   \
	X(receive_option_byte_proc)                                                                                        \
	X(reset_proc)                                                                                                      \
	X(reset_wait_proc)                                                                                                 \
	X(send_scancode_set_proc)

static unsigned char (*const PROGMEM procedures[])(Keyboard *self, unsigned char parameter) = {
#define X(name) name,
	PROCEDURES
#undef X
};

/* clang-format off */

enum
{
#define X(name) name##_index,
	PROCEDURES
#undef X
	end_proc_index
};

/* clang-format on */

static const unsigned char PROGMEM next_states[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) (next_state),
#include "keyboard_states.def"
#undef DEFINE_STATE
};

static const unsigned char PROGMEM parameters[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) (parameter),
#include "keyboard_states.def"
#undef DEFINE_STATE
};

static const unsigned char PROGMEM procedure_offsets[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) PACK_71(procedure##_index * 2, (receive)),
#include "keyboard_states.def"
#undef DEFINE_STATE
};

void keyboard_work(Keyboard *const self, const unsigned char time_delta)
{
	if (self->reset_timer > time_delta)
	{
		self->reset_timer -= time_delta;
	}
	else
	{
		self->reset_timer = 0;
	}

	if (self->bat_timer > time_delta)
	{
		self->bat_timer -= time_delta;
	}
	else
	{
		const unsigned char next_state = pgm_read_byte_near((unsigned short)next_states + self->state);
		const unsigned char parameter = pgm_read_byte_near((unsigned short)parameters + self->state);
		const unsigned char procedure_offset_receive =
			pgm_read_byte_near((unsigned short)procedure_offsets + self->state);
		const unsigned char procedure_offset = UNPACK_71_0(procedure_offset_receive);
		const unsigned char procedure_receive = UNPACK_71_1(procedure_offset_receive);
		unsigned char (*const procedure)(Keyboard *, unsigned char) = (unsigned char (*)(
			Keyboard *, unsigned char))pgm_read_word_near((unsigned short)procedures + procedure_offset);
		self->bat_timer = 0;
		if (PS2_KEYBOARD_KEY_RESERVED != self->last_typematic_key)
		{
			if (self->typematic_timer <= time_delta)
			{
				self->typematic_timer += self->typematic_period;
				/* TODO: do not insert scancode if transmission is inhibited */
				insert_scancode(self, self->last_typematic_key, 0, 1);
			}
			self->typematic_timer -= time_delta;
		}
		if (ps2_can_receive(self->clock_mask, self->data_mask))
		{
			unsigned char byte;
			if (ps2_receive(self->clock_mask, self->data_mask, &byte))
			{
				if (byte >= 0xed)
				{
					self->state = byte - 0xed;
				}
				else if (procedure_receive)
				{
					self->option_byte = byte;
					if (procedure(self, parameter))
					{
						self->state = next_state;
					}
				}
				else
				{
					self->state = STATE_INVALID_COMMAND_0;
				}
			}
			else
			{
				self->state = STATE_INVALID_COMMAND_0;
			}
		}
		else if (!procedure_receive)
		{
			if (procedure(self, parameter))
			{
				self->state = next_state;
			}
		}
	}
}

void keyboard_set_key_state(Keyboard *const self, const Ps2KeyboardKeyCode key_code, const unsigned char release)
{
	insert_scancode(self, key_code, release, 0);
}

unsigned char keyboard_get_leds(Keyboard *const self)
{
	return self->bat_timer > 0 ? 0x7 : self->leds;
}
