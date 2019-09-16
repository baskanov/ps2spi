#include <ps2/ps2.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

unsigned char ps2_can_send(const unsigned char clock_mask, const unsigned char data_mask)
{
	return (PIND & (clock_mask | data_mask)) == (clock_mask | data_mask);
}

unsigned char ps2_can_receive(const unsigned char clock_mask, const unsigned char data_mask)
{
	return (PIND & (clock_mask | data_mask)) == clock_mask;
}

static void wait_for_tick()
{
	TIFR = 1u << OCF2;
	while (!(TIFR & (1u << OCF2)))
		;
}

static unsigned char send_bit(const unsigned char bit, const unsigned char clock_mask, const unsigned char data_mask)
{
	unsigned char next_ddrd = DDRD;

	next_ddrd &= ~data_mask;
	if (!bit)
		next_ddrd |= data_mask;
	wait_for_tick();
	DDRD = next_ddrd;
	if (!(PIND & clock_mask))
	{
		DDRD &= ~data_mask;
		return 0;
	}

	next_ddrd |= clock_mask;
	wait_for_tick();
	DDRD = next_ddrd;

	wait_for_tick();

	next_ddrd &= ~clock_mask;
	wait_for_tick();
	DDRD = next_ddrd;

	return 1;
}

/* clang-format off */

/*
 *
 * 0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0
 *  _____       _____       _____       _____       _____       _____       _____       _____       _____       _____       _____
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|
 *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 *  __             ___________ ___________ ___________ ___________ ___________ ___________ ___________ ___________ ___________ _________
 *    |           |           |           |           |           |           |           |           |           |           |
 *    |   Start   |   Bit 0   |   Bit 1   |   Bit 2   |   Bit 3   |   Bit 4   |   Bit 5   |   Bit 6   |   Bit 7   |  Parity   |   Stop
 *    |___________|___________|___________|___________|___________|___________|___________|___________|___________|___________|
 *
 */

/* clang-format on */

unsigned char ps2_send(const unsigned char clock_mask, const unsigned char data_mask, const unsigned char byte)
{
	unsigned char bit_index;
	unsigned char parity = 1;
	unsigned char shifted_byte = byte;

	/* start bit */
	if (!send_bit(0, clock_mask, data_mask))
		return 0;

	for (bit_index = 0; bit_index < 8; ++bit_index)
	{
		const unsigned char bit_value = shifted_byte & 1u;
		shifted_byte >>= 1u;
		parity ^= bit_value;

		if (!send_bit(bit_value, clock_mask, data_mask))
			return 0;
	}

	if (!send_bit(parity, clock_mask, data_mask))
		return 0;

	/* stop bit */
	if (!send_bit(1, clock_mask, data_mask))
		return 0;

	return 1;
}

static unsigned char receive_bit(const unsigned char clock_mask, const unsigned char data_mask)
{
	unsigned char next_ddrd = DDRD;
	unsigned char result;

	wait_for_tick();
	result = (PIND & data_mask) ? 1 : 0;

	next_ddrd |= clock_mask;
	wait_for_tick();
	DDRD = next_ddrd;

	wait_for_tick();

	next_ddrd &= ~clock_mask;
	wait_for_tick();
	DDRD = next_ddrd;

	return result;
}

/* clang-format off */

/*
 *
 * 0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3  0
 *  _____       _____       _____       _____       _____       _____       _____       _____       _____       _____       _____
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|  |  |_____|
 *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 *           ___________ ___________ ___________ ___________ ___________ ___________ ___________ ___________ ___________ ______
 *          |           |           |           |           |           |           |           |           |           |      |
 *  Start   |   Bit 0   |   Bit 1   |   Bit 2   |   Bit 3   |   Bit 4   |   Bit 5   |   Bit 6   |   Bit 7   |  Parity   | Stop |   LC
 *  ________|___________|___________|___________|___________|___________|___________|___________|___________|___________|      |________
 *
 */

/* clang-format on */

unsigned char ps2_receive(const unsigned char clock_mask, const unsigned char data_mask, unsigned char *const output)
{
	unsigned char result = 0;
	unsigned char bit_index;
	unsigned char byte = 0;
	unsigned char parity = 0;

	/* start bit */
	receive_bit(clock_mask, data_mask);

	for (bit_index = 0; bit_index < 8; ++bit_index)
	{
		const unsigned char bit_value = receive_bit(clock_mask, data_mask);
		byte >>= 1u;
		if (bit_value)
			byte |= 0x80;
		parity ^= bit_value;
	}

	/* parity bit */
	parity ^= receive_bit(clock_mask, data_mask);

	/* stop bit */
	wait_for_tick();
	if (!(PIND & data_mask))
	{
		/* framing error */
		unsigned char next_ddrd = DDRD;
		/* TODO: limit cycle count to prevent WDT reset */
		for (;;)
		{
			next_ddrd |= ~clock_mask;
			wait_for_tick();
			DDRD = next_ddrd;

			wait_for_tick();

			next_ddrd &= ~clock_mask;
			wait_for_tick();
			DDRD = next_ddrd;

			wait_for_tick();
			if (PIND & data_mask)
				break;
		}
	}
	else
	{
		*output = byte;
		result = 1;
	}

	{
		unsigned char next_ddrd = DDRD;

		/* line control */
		next_ddrd |= data_mask;
		DDRD = next_ddrd;

		next_ddrd |= clock_mask;
		wait_for_tick();
		DDRD = next_ddrd;

		wait_for_tick();

		next_ddrd &= ~clock_mask;
		wait_for_tick();
		DDRD = next_ddrd;

		next_ddrd &= ~data_mask;
		wait_for_tick();
		DDRD = next_ddrd;
	}

	if (!parity)
		return 0;

	return result;
}
