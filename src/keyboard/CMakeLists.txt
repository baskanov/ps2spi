set(SOURCES include/keyboard/keyboard.h src/keyboard_key_types.def src/keyboard_selectors.def src/keyboard_sequences.def src/keyboard_states.def src/keyboard.c)

add_library(keyboard STATIC ${SOURCES})

target_compile_options(keyboard PRIVATE -mmcu=${MCU} -pedantic -Wall -Werror)
target_compile_features(keyboard PRIVATE c_std_90)
target_include_directories(keyboard PUBLIC include)
target_link_libraries(keyboard PRIVATE ps2spi_interface ps2)

add_executable(keyboard_test ${SOURCES} test/main.c)

target_compile_options(keyboard_test PRIVATE -mmcu=${TEST_MCU} -pedantic -Wall -Werror)
target_compile_features(keyboard_test PRIVATE c_std_90)
set_target_properties(keyboard_test PROPERTIES LINK_FLAGS -mmcu=${TEST_MCU})
target_include_directories(keyboard_test PRIVATE include)
target_link_libraries(keyboard_test PRIVATE ps2spi_interface ps2_mock)

add_test(
    keyboard_test
    avr-gdb
    --batch
    --command
    ${CMAKE_CURRENT_SOURCE_DIR}/../ps2_mock/test.gdb
    ${CMAKE_CURRENT_BINARY_DIR}/keyboard_test
)
set_tests_properties(keyboard_test PROPERTIES
    PASS_REGULAR_EXPRESSION "TEST_PASSED"
    FAIL_REGULAR_EXPRESSION "TEST_FAILED"
)
