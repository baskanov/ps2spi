#ifndef PS2_MOCK_H
#define PS2_MOCK_H

enum
{
	PS2_MOCK_END,
	PS2_MOCK_CAN_SEND,
	PS2_MOCK_CAN_RECEIVE,
	PS2_MOCK_SEND,
	PS2_MOCK_RECEIVE
};

typedef struct
{
	unsigned char function;
	unsigned char parameters[3];
	unsigned char return_values[2];
} Ps2MockExpectedCall;

void ps2_mock_set_expected_calls(const Ps2MockExpectedCall *calls);
void ps2_mock_check_remainder(void);
void ps2_mock_report_failure(void);
void ps2_mock_report_success(void);

#endif /* PS2_MOCK_H */
