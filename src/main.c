#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include <keyboard/keyboard.h>
#include <mouse/mouse.h>

FUSES = {
	/* 8Mhz Internal RC Oscillator, 4.0v Brown-out Detection */
	FUSE_SUT_CKSEL0 & FUSE_SUT_CKSEL1 & FUSE_SUT_CKSEL3 & FUSE_SUT_CKSEL4 & FUSE_SUT_CKSEL5 & FUSE_BODEN &
		FUSE_BODLEVEL,
	/* Serial Downloading, Watchdog Timer always enabled */
	FUSE_SPIEN &FUSE_WTDON,
};

#define KEYBOARD_CLOCK PORTD0
#define KEYBOARD_DATA PORTD1
#define MOUSE_CLOCK PORTD2
#define MOUSE_DATA PORTD3

/*#define KEYBOARD_CLOCK PORTD2
#define KEYBOARD_DATA PORTD3
#define MOUSE_CLOCK PORTD0
#define MOUSE_DATA PORTD1*/

static Keyboard keyboard;
static Mouse mouse;

static const unsigned char command_lengths[] = {
	0,
	1,
	1,
	6,
};

static void handle_spi(void)
{
	unsigned char command = 0xff;
	unsigned char length = 1;
	unsigned char offset = 0;
	unsigned char buffer[6];

	if (!(SPSR & (1u << SPIF)))
		return;

	for (;;)
	{
		const unsigned char byte = SPDR;
		if (byte >= 0xfc)
		{
			SPDR = 0xfa;
			command = byte - 0xfc;
			length = command_lengths[command];
			offset = 0;
		}
		else
		{
			SPDR = 0;
			buffer[offset] = byte;
			++offset;
		}
		if (offset == length)
			break;
		while (!(SPSR & (1u << SPIF)))
			;
	}

	switch (command)
	{
	case 0:
		while (!(SPSR & (1u << SPIF)))
			;
		SPDR = keyboard_get_leds(&keyboard);
		break;
	case 1:
		keyboard_set_key_state(&keyboard, buffer[0], 0);
		break;
	case 2:
		keyboard_set_key_state(&keyboard, buffer[0], 1);
		break;
	case 3:
	{
		short delta_x = buffer[0] | (buffer[1] << 7);
		short delta_y = buffer[2] | (buffer[3] << 7);
		char delta_z = buffer[4];
		const unsigned char buttons = buffer[5];
		if (delta_x & 0x2000)
			delta_x |= 0xc000;
		if (delta_y & 0x2000)
			delta_y |= 0xc000;
		if (delta_z & 0x40)
			delta_z |= 0x80;
		mouse_update(&mouse, delta_x, delta_y, delta_z, buttons);
		break;
	}
	}
}

int main(void)
{
	/* Load 8Mhz RC oscillator calibration byte */
	OSCCAL = eeprom_read_byte((const uint8_t *)3);

	/* Enable pull-ups for unconnected pins */
	PORTB = 0xff;
	PORTC = 0xff;
	PORTD = (unsigned char)~((1u << KEYBOARD_CLOCK) | (1u << KEYBOARD_DATA) | (1u << MOUSE_CLOCK) | (1u << MOUSE_DATA));
	/*PORTD = (unsigned char)~((1u << MOUSE_CLOCK) | (1u << MOUSE_DATA));*/

	/* SPI slave mode */
	SPCR = (1u << SPE);

	/* Configure MISO pin as output */
	DDRB = (1u << DDRB4);

	/* Timer 1 */
	/* System clock divided by 8, CTC mode */
	TCCR1B = (1u << CS11) | (1u << WGM12);
	/* 4ms period */
	OCR1A = 3999;

	/* Timer 2 */
	/* System clock divided by 8, CTC mode */
	TCCR2 = (1u << CS21) | (1u << WGM21);
	/* 20us period */
	OCR2 = 19;

	keyboard_init(&keyboard, 1u << KEYBOARD_CLOCK, 1u << KEYBOARD_DATA);
	mouse_init(&mouse, 1u << MOUSE_CLOCK, 1u << MOUSE_DATA);

	for (;;)
	{
		const unsigned char timer_flag = TIFR & (1u << OCF1A);
		const unsigned char time_delta = timer_flag ? 1u : 0u;
		TIFR = timer_flag;

		handle_spi();

		/* TODO: check PS/2 VCC line */

		keyboard_work(&keyboard, time_delta);
		mouse_work(&mouse, time_delta);

		wdt_reset();
	}
}
