#include <stddef.h>
#include <stdlib.h>

#include <keyboard/keyboard.h>
#include <ps2_mock/ps2_mock.h>
#include <ps2spi/ps2_keyboard_codes_enum.h>

static void test_bat(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		keyboard_work(&keyboard, 120);
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
}

static void init(Keyboard *keyboard)
{
	const Ps2MockExpectedCall expected_calls[] = {
		{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
		{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
		{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
		{ PS2_MOCK_END },
	};

	keyboard_init(keyboard, 0x1, 0x2);

	ps2_mock_set_expected_calls(expected_calls);
	keyboard_work(keyboard, 120);
	keyboard_work(keyboard, 255);
	ps2_mock_check_remainder();
}

static void check_input(Keyboard *keyboard, const unsigned char *data, const unsigned char size)
{
	unsigned char i;
	for (i = 0; i < size; ++i)
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, data[i] } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void check_output(Keyboard *keyboard, const unsigned char *expected_data, const unsigned char expected_size)
{
	unsigned char i;
	for (i = 0; i < expected_size; ++i)
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, expected_data[i] }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void delay(Keyboard *keyboard, const unsigned char ticks)
{
	const Ps2MockExpectedCall expected_calls[] = {
		{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
		{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 0 } },
		{ PS2_MOCK_END },
	};
	ps2_mock_set_expected_calls(expected_calls);
	keyboard_work(keyboard, ticks);
	ps2_mock_check_remainder();
}

static void test_pause_key(void)
{
	const unsigned char make_expected[] = {
		0xe1, 0x14, 0x77, 0xe1, 0xf0, 0x14, 0xf0, 0x77,
	};
	Keyboard keyboard;
	init(&keyboard);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_PAUSE, 0);
	check_output(&keyboard, make_expected, sizeof(make_expected));
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_PAUSE, 1);
	check_output(&keyboard, NULL, 0);
}

static void test_up_key_numlock_on(void)
{
	const unsigned char num_lock_expected[] = {
		0x77,
		0xf0,
		0x77,
	};
	const unsigned char up_expected[] = {
		0xe0, 0x12, 0xe0, 0x75, 0xe0, 0xf0, 0x75, 0xe0, 0xf0, 0x12,
	};
	Keyboard keyboard;
	init(&keyboard);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 1);
	check_output(&keyboard, num_lock_expected, sizeof(num_lock_expected));
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 1);
	check_output(&keyboard, up_expected, sizeof(up_expected));
}

static void test_up_key_repeat(void)
{
	const unsigned char up_expected[] = {
		0xe0,
		0x75,
	};
	Keyboard keyboard;
	init(&keyboard);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	check_output(&keyboard, up_expected, sizeof(up_expected));
	delay(&keyboard, 130);
	check_output(&keyboard, up_expected, sizeof(up_expected));
}

static void test_up_key_numlock_on_repeat(void)
{
	const unsigned char num_lock_expected[] = {
		0x77,
		0xf0,
		0x77,
	};
	const unsigned char up_expected[] = {
		0xe0,
		0x12,
		0xe0,
		0x75,
	};
	const unsigned char repeat_expected[] = {
		0xe0,
		0x75,
	};
	Keyboard keyboard;
	init(&keyboard);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 1);
	check_output(&keyboard, num_lock_expected, sizeof(num_lock_expected));
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	check_output(&keyboard, up_expected, sizeof(up_expected));
	delay(&keyboard, 130);
	check_output(&keyboard, repeat_expected, sizeof(repeat_expected));
}

static void test_set_leds(void)
{
	const unsigned char command = 0xed;
	const unsigned char ack = 0xfa;
	const unsigned char leds = 0x5;
	Keyboard keyboard;
	init(&keyboard);
	check_input(&keyboard, &command, 1);
	check_output(&keyboard, &ack, 1);
	check_input(&keyboard, &leds, 1);
	check_output(&keyboard, &ack, 1);
	if (keyboard_get_leds(&keyboard) != leds)
	{
		ps2_mock_report_failure();
	}
}

static void test_set_typematic_rate_delay(void)
{
	const unsigned char command = 0xf3;
	const unsigned char ack = 0xfa;
	const unsigned char typematic_rate_delay = 0x7f;
	Keyboard keyboard;
	init(&keyboard);
	check_input(&keyboard, &command, 1);
	check_output(&keyboard, &ack, 1);
	check_input(&keyboard, &typematic_rate_delay, 1);
	check_output(&keyboard, &ack, 1);
}

int main(void)
{
	test_bat();
	test_pause_key();
	test_up_key_numlock_on();
	test_up_key_repeat();
	test_up_key_numlock_on_repeat();
	test_set_leds();
	test_set_typematic_rate_delay();

	ps2_mock_report_success();
}
