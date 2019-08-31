#include <mouse/mouse.h>

#include <ps2/ps2.h>

#include <avr/pgmspace.h>

enum
{
#define DEFINE_STATE(name, next_state, parameter, procedure_offset, receive) name,
#include "mouse_states.def"
#undef DEFINE_STATE
	STATE_COUNT
};

static void set_defaults(Mouse *const self)
{
	self->sample_rate = 100;
	self->resolution = 2;
	self->mode = MOUSE_MODE_STREAM;
	self->enabled = 0;
	self->scaling = 0;
	self->protocol = MOUSE_PROTOCOL_DEFAULT;
	self->magic_sequence_offset = 0;
}

static void reset(Mouse *const self)
{
	self->delta_x = 0;
	self->delta_y = 0;
	self->delta_z = 0;
	self->buttons = 0;
	self->state = STATE_SEND_COMPLETION_CODE;
	self->echo = 0;
	set_defaults(self);
}

void mouse_init(Mouse *const self, const unsigned char clock_mask, const unsigned char data_mask)
{
	self->clock_mask = clock_mask;
	self->data_mask = data_mask;
	reset(self);
}

#define PACK_71(a, b) ((a) | ((b) << 7) | ((((a) > 0x7f) || ((b) > 0x1)) ? 0x100 : 0))
#define UNPACK_71_0(a) ((unsigned char)((a) & 0x7f))
#define UNPACK_71_1(a) ((unsigned char)((a) >> 7))

static unsigned char send_packet_byte_proc(Mouse *const self, const unsigned char parameter)
{
	if (!ps2_can_send(self->clock_mask, self->data_mask))
		return 0;
	if (!ps2_send(self->clock_mask, self->data_mask, self->packet[self->current_packet_offset]))
		return 0;
	++self->current_packet_offset;
	return self->current_packet_offset >= self->packet_size;
}

static unsigned char send_byte_proc(Mouse *const self, const unsigned char parameter)
{
	self->packet[0] = parameter;
	self->packet_size = 1;
	self->current_packet_offset = 0;

	return send_packet_byte_proc(self, 0);
}

static unsigned char set_scaling_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;
	self->scaling = parameter;
	return 1;
}

static unsigned char set_resolution_proc(Mouse *const self, const unsigned char parameter)
{
	/* TODO: validate option byte value */
	self->resolution = self->option_byte;
	return 1;
}

static unsigned char send_status_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;

	self->packet[0] = (self->mode << 6u) | (self->enabled << 5u) | (self->scaling << 4u) | ((self->buttons << 2u) & 0x4) | ((self->buttons >> 1u) & 0x3);
	self->packet[1] = self->resolution;
	self->packet[2] = self->sample_rate;
	self->packet_size = 3;
	
	self->current_packet_offset = 0;

	return 1;
}

static unsigned char set_mode_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;
	self->mode = parameter;
	return 1;
}

static unsigned char send_movement_packet_proc(Mouse *const self, const unsigned char parameter)
{
	/* TODO: set overflow flags */
	/* TODO: apply scaling */
	short scaled_delta_x = self->delta_x;
	short scaled_delta_y = self->delta_y;
	unsigned short delta_mask = (unsigned short)~0u;
	if (!(self->resolution & 1))
	{
		scaled_delta_x >>= 1;
		scaled_delta_y >>= 1;
		delta_mask <<= 1;
	}
	if (!(self->resolution & 2))
	{
		scaled_delta_x >>= 2;
		scaled_delta_y >>= 2;
		delta_mask <<= 2;
	}
	delta_mask = ~delta_mask;
	self->packet[0] = (self->buttons & 0x7) | 0x8;
	if (scaled_delta_x & 0x100)
		self->packet[0] |= 0x10;
	if (scaled_delta_y & 0x100)
		self->packet[0] |= 0x20;
	self->packet[1] = (unsigned char)(unsigned short)scaled_delta_x;
	self->packet[2] = (unsigned char)(unsigned short)scaled_delta_y;
	if (self->protocol == MOUSE_PROTOCOL_5_BUTTON)
	{
		self->packet[3] = (self->delta_z & 0xf) | ((self->buttons << 1) & 0x30);
		self->packet_size = 4;
	}
	else if (self->protocol == MOUSE_PROTOCOL_3D)
	{
		self->packet[3] = self->delta_z;
		self->packet_size = 4;
	}
	else
	{
		self->packet_size = 3;
	}

	self->delta_x &= delta_mask;
	self->delta_y &= delta_mask;
	self->delta_z = 0;
	self->pending = 0;

	self->current_packet_offset = 0;
	send_packet_byte_proc(self, 0);

	return 1;
}

static unsigned char set_echo_mode_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;

	self->echo = parameter;

	return 1;
}

static unsigned char send_id_proc(Mouse *const self, const unsigned char parameter)
{
	return send_byte_proc(self, self->protocol);
}

static unsigned char set_sample_rate_proc(Mouse *const self, const unsigned char parameter)
{
	/* TODO: validate sample rate value */
	/* TODO: compute period */
	self->sample_rate = self->option_byte;
	return 1;
}

static unsigned char set_enabled_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;
	self->enabled = parameter;
	return 1;
}

static unsigned char set_defaults_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;
	set_defaults(self);
	return 1;
}

static unsigned char resend_proc(Mouse *const self, const unsigned char parameter)
{
	self->current_packet_offset = 0;
	send_packet_byte_proc(self, 0);
	return 1;
}

static unsigned char reset_proc(Mouse *const self, const unsigned char parameter)
{
	if (!send_byte_proc(self, 0xfa))
		return 0;
	reset(self);
	return 0;
}

static unsigned char idle_proc(Mouse *const self, const unsigned char parameter)
{
	if (MOUSE_MODE_STREAM == self->mode)
	{
		/* TODO: wait for the timer to expire */
		if (self->pending && self->enabled)
		{
			send_movement_packet_proc(self, 0);
			self->state = STATE_SEND_PACKET_BYTE;
			return 0;
		}
	}
	return 1;
}

static unsigned char echo_proc(Mouse *const self, const unsigned char parameter)
{
	if (0xec == self->option_byte)
	{
		if (set_echo_mode_proc(self, 0))
			self->state = STATE_IDLE;
		else
			self->state = STATE_CLEAR_ECHO_MODE;
		return 0;
	}
	else if (0xff == self->option_byte)
	{
		if (reset_proc(self, 0))
			self->state = STATE_IDLE;
		else
			self->state = STATE_RESET;
		return 0;
	}
	else
	{
		send_byte_proc(self, self->option_byte);
	}
	return 1;
}

static unsigned char send_completion_code_proc(Mouse *const self, const unsigned char parameter)
{
	self->packet[0] = 0xaa;
	self->packet[1] = 0x00;
	self->packet_size = 2;

	self->current_packet_offset = 0;
	send_packet_byte_proc(self, 0);

	return 1;
}

static unsigned char no_op_proc(Mouse *const self, const unsigned char parameter)
{
	return 1;
}

#define PROCEDURES               \
	X(send_byte_proc)            \
	X(set_scaling_proc)          \
	X(set_resolution_proc)       \
	X(send_status_proc)          \
	X(set_mode_proc)             \
	X(send_movement_packet_proc) \
	X(set_echo_mode_proc)        \
	X(send_id_proc)              \
	X(set_sample_rate_proc)      \
	X(set_enabled_proc)          \
	X(set_defaults_proc)         \
	X(resend_proc)               \
	X(reset_proc)                \
	X(idle_proc)                 \
	X(send_packet_byte_proc)     \
	X(send_completion_code_proc) \
	X(echo_proc)                 \
	X(no_op_proc)

static unsigned char (*const PROGMEM procedures[])(Mouse *self, unsigned char parameter) = {
#define X(name) name,
PROCEDURES
#undef X
};

enum
{
#define X(name) name##_index,
PROCEDURES
#undef X
	end_proc_index
};

static const unsigned char PROGMEM next_states[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) (next_state),
#include "mouse_states.def"
#undef DEFINE_STATE
};

static const unsigned char PROGMEM parameters[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) (parameter),
#include "mouse_states.def"
#undef DEFINE_STATE
};

static const unsigned char PROGMEM procedure_offsets[] = {
#define DEFINE_STATE(name, next_state, parameter, procedure, receive) PACK_71(procedure##_index * 2, (receive)),
#include "mouse_states.def"
#undef DEFINE_STATE
};

static const unsigned char magic_sequences[] = {
	0xf3,
	0xc8,
	0xf3,
	0x64,
	0xf3,
	0x50,

	0xf3,
	0xc8,
	0xf3,
	0xc8,
	0xf3,
	0x50,
};

void mouse_work(Mouse *const self, const unsigned char time_delta)
{
	const unsigned char next_state = pgm_read_byte_near((unsigned short)next_states + self->state);
	const unsigned char parameter = pgm_read_byte_near((unsigned short)parameters + self->state);
	const unsigned char procedure_offset_receive = pgm_read_byte_near((unsigned short)procedure_offsets + self->state);
	const unsigned char procedure_offset = UNPACK_71_0(procedure_offset_receive);
	const unsigned char procedure_receive = UNPACK_71_1(procedure_offset_receive);
	unsigned char (*const procedure)(Mouse *, unsigned char) = (unsigned char (*)(Mouse *, unsigned char))pgm_read_word_near((unsigned short)procedures + procedure_offset);
	if (ps2_can_receive(self->clock_mask, self->data_mask))
	{
		unsigned char byte;
		if (ps2_receive(self->clock_mask, self->data_mask, &byte))
		{
			if (byte == magic_sequences[self->magic_sequence_offset])
			{
				++self->magic_sequence_offset;
				if (6 == self->magic_sequence_offset)
				{
					self->protocol = MOUSE_PROTOCOL_3D;
				}
				else if (12 == self->magic_sequence_offset)
				{
					self->protocol = MOUSE_PROTOCOL_5_BUTTON;
					self->magic_sequence_offset = 6;
				}
			}
			else
			{
				if (self->magic_sequence_offset < 6)
					self->magic_sequence_offset = 0;
				else
					self->magic_sequence_offset = 6;
			}

			if (self->echo)
			{
				self->option_byte = byte;
				self->state = STATE_ECHO_SEND_BYTE;
			}
			else if (byte >= 0xe6)
			{
				self->state = byte - 0xe6;
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

void mouse_update(Mouse *const self, const short delta_x, const short delta_y, const char delta_z, const unsigned char buttons)
{
	self->delta_x += delta_x;
	self->delta_y += delta_y;
	self->delta_z += delta_z;
	self->buttons = buttons;
	self->pending = 1;
}
