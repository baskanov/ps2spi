target sim
load

break ps2_mock_report_failure
commands
silent
echo TEST_FAILED\n
backtrace
end

break ps2_mock_report_success
commands
silent
echo TEST_PASSED\n
end

run
