#include <stddef.h>

#include <ps2/ps2.h>
#include <ps2_mock/ps2_mock.h>

static const Ps2MockExpectedCall *expected_calls = NULL;
static unsigned char expected_call_index = 0;

static void halt(void)
{
	for (;;)
		;
}

static void report_unexpected_call(void)
{
	ps2_mock_report_failure();
}

static void report_unexpected_parameter(void)
{
	ps2_mock_report_failure();
}

static void report_missing_call(void)
{
	ps2_mock_report_failure();
}

static unsigned char check_call(
	const unsigned char function,
	const unsigned char parameter0,
	const unsigned char parameter1,
	const unsigned char parameter2,
	unsigned char *const output_ptr)
{
	if (!expected_calls)
		report_unexpected_call();

	if (expected_calls[expected_call_index].function != function)
		report_unexpected_call();

	if (expected_calls[expected_call_index].parameters[0] != parameter0)
		report_unexpected_parameter();
	if (expected_calls[expected_call_index].parameters[1] != parameter1)
		report_unexpected_parameter();
	if (expected_calls[expected_call_index].parameters[2] != parameter2)
		report_unexpected_parameter();

	if (output_ptr)
		*output_ptr = expected_calls[expected_call_index].return_values[1];

	{
		const unsigned char result = expected_calls[expected_call_index].return_values[0];

		++expected_call_index;

		return result;
	}
}

unsigned char ps2_can_send(const unsigned char clock_mask, const unsigned char data_mask)
{
	return check_call(PS2_MOCK_CAN_SEND, clock_mask, data_mask, 0, NULL);
}

unsigned char ps2_can_receive(const unsigned char clock_mask, const unsigned char data_mask)
{
	return check_call(PS2_MOCK_CAN_RECEIVE, clock_mask, data_mask, 0, NULL);
}

unsigned char ps2_send(const unsigned char clock_mask, const unsigned char data_mask, const unsigned char byte)
{
	return check_call(PS2_MOCK_SEND, clock_mask, data_mask, byte, NULL);
}

unsigned char ps2_receive(const unsigned char clock_mask, const unsigned char data_mask, unsigned char *const output)
{
	return check_call(PS2_MOCK_RECEIVE, clock_mask, data_mask, 0, output);
}

void ps2_mock_set_expected_calls(const Ps2MockExpectedCall *const calls)
{
	expected_calls = calls;
	expected_call_index = 0;
}

void ps2_mock_check_remainder(void)
{
	if (PS2_MOCK_END != expected_calls[expected_call_index].function)
		report_missing_call();
}

void ps2_mock_report_failure(void)
{
	halt();
}

void ps2_mock_report_success(void)
{
	halt();
}
