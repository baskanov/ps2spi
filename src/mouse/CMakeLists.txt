set(SOURCES include/mouse/mouse.h src/mouse_states.def src/mouse.c)

add_library(mouse STATIC ${SOURCES})

target_compile_options(mouse PRIVATE -mmcu=${MCU} -pedantic -Wall -Werror)
target_compile_features(mouse PRIVATE c_std_90)
target_include_directories(mouse PUBLIC include)
target_link_libraries(mouse PRIVATE ps2)

add_executable(mouse_test ${SOURCES} test/main.c)

target_compile_options(mouse_test PRIVATE -mmcu=${TEST_MCU} -pedantic -Wall -Werror)
target_compile_features(mouse_test PRIVATE c_std_90)
set_target_properties(mouse_test PROPERTIES LINK_FLAGS -mmcu=${TEST_MCU})
target_include_directories(mouse_test PRIVATE include)
target_link_libraries(mouse_test PRIVATE ps2_mock)

add_test(
    mouse_test
    avr-gdb
    --batch
    --command
    ${CMAKE_CURRENT_SOURCE_DIR}/../ps2_mock/test.gdb
    ${CMAKE_CURRENT_BINARY_DIR}/mouse_test
)
set_tests_properties(mouse_test PROPERTIES
PASS_REGULAR_EXPRESSION "TEST_PASSED"
FAIL_REGULAR_EXPRESSION "TEST_FAILED"
)
