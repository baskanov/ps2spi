set(SOURCES include/keyboard/keyboard.h src/keyboard_key_types.def src/keyboard_selectors.def src/keyboard_sequences.def src/keyboard_states.def src/keyboard.c)

add_library(keyboard STATIC ${SOURCES})

target_compile_options(keyboard PRIVATE -mmcu=atmega8a -pedantic -Wall -Werror)
target_compile_features(keyboard PRIVATE c_std_90)
target_include_directories(keyboard PUBLIC include)
target_link_libraries(keyboard PRIVATE ps2spi_interface ps2)

add_executable(keyboard_test ${SOURCES} test/main.c)

target_compile_options(keyboard_test PRIVATE -mmcu=atmega16a -pedantic -Wall -Werror)
target_compile_features(keyboard_test PRIVATE c_std_90)
set_target_properties(keyboard_test PROPERTIES LINK_FLAGS -mmcu=atmega16a)
target_include_directories(keyboard_test PRIVATE include)
target_link_libraries(keyboard_test PRIVATE ps2_mock)
