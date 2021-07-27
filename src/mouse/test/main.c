#include <stddef.h>
#include <stdlib.h>

#include <mouse/mouse.h>
#include <ps2_mock/ps2_mock.h>

static void test_bat(void)
{
	Mouse mouse;
	mouse_init(&mouse, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2, 0 }, { 0, 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2, 0 }, { 1, 0 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1, 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x00 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
}

void test_movement(void)
{
	Mouse mouse;
	mouse_init(&mouse, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			/* clang-format off */
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x00 }, { 1 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xf4 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
			/* clang-format on */
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	mouse_update(&mouse, 1, -1, 1, 5);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x2d }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 255);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x00 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xff }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
}

void test_echo(void)
{
	Mouse mouse;
	mouse_init(&mouse, 0x1, 0x2);
	{
		const Ps2MockExpectedCall expected_calls[] = {
			/* clang-format off */
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xaa }, { 1 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x00 }, { 1 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xf4 } },
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xfa }, { 1 } },
			{ PS2_MOCK_END },
			/* clang-format on */
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xee } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
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
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xf2 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0xf2 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xec } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
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
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_RECEIVE, { 0x1, 0x2 }, { 1, 0xf2 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
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
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_CAN_SEND, { 0x1, 0x2 }, { 1 } },
			{ PS2_MOCK_SEND, { 0x1, 0x2, 0x00 }, { 1 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
	{
		const Ps2MockExpectedCall expected_calls[] = {
			{ PS2_MOCK_CAN_RECEIVE, { 0x1, 0x2 }, { 0 } },
			{ PS2_MOCK_END },
		};
		ps2_mock_set_expected_calls(expected_calls);
		mouse_work(&mouse, 0);
		ps2_mock_check_remainder();
	}
}

int main(void)
{
	test_bat();
	test_movement();
	test_echo();

	ps2_mock_report_success();
}
