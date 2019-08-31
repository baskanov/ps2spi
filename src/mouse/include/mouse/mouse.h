#ifndef MOUSE_H
#define MOUSE_H

typedef enum
{
	MOUSE_PROTOCOL_DEFAULT = 0,
	MOUSE_PROTOCOL_3D = 3,
	MOUSE_PROTOCOL_5_BUTTON = 4
} MouseProtocol;

typedef enum
{
	MOUSE_MODE_STREAM = 0,
	MOUSE_MODE_REMOTE = 1
} MouseMode;

typedef struct
{
	unsigned char clock_mask;
	unsigned char data_mask;
	short delta_x;
	short delta_y;
	char delta_z;
	unsigned char buttons;
	unsigned char pending;
	unsigned char state;
	unsigned char option_byte;
	unsigned char scaling;
	unsigned char resolution;
	unsigned char sample_rate;
	unsigned char magic_sequence_offset;
	unsigned char echo;
	unsigned char enabled;
	MouseMode mode;
	MouseProtocol protocol;
	unsigned char packet[4];
	unsigned char packet_size;
	unsigned char current_packet_offset;
} Mouse;

void mouse_init(Mouse *self, unsigned char clock_mask, unsigned char data_mask);
void mouse_work(Mouse *self, unsigned char time_delta);
void mouse_update(Mouse *self, short delta_x, short delta_y, char delta_z, unsigned char buttons);

#endif /* MOUSE_H */
