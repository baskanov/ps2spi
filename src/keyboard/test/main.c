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

static void test_pause_key(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_PAUSE, 0);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe1 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x14 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe1 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x14 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_PAUSE, 1);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void test_up_key_numlock_on(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 1);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 1);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x12 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x12 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void test_up_key_repeat(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 130);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void test_up_key_numlock_on_repeat(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 0);
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_NUMLOCK, 1);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x77 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	keyboard_set_key_state(&keyboard, PS2_KEYBOARD_KEY_UP, 0);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x12 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xe0 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 130);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x75 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void test_set_leds(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xed } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0x5 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	if (keyboard_get_leds(&keyboard) != 0x5)
	{
		for (;;)
			;
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
}

static void test_set_typematic_rate_delay(void)
{
	Keyboard keyboard;
	keyboard_init(&keyboard, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 120);
		keyboard_work(&keyboard, 255);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xf3 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0x7f } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		keyboard_work(&keyboard, 0);
		ps2_mock_check_remainder();
	}
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
