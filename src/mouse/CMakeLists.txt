set(SOURCES include/mouse/mouse.h src/mouse_states.def src/mouse.c)

add_library(mouse STATIC ${SOURCES})

target_compile_options(mouse PRIVATE -mmcu=atmega8a -pedantic -Wall -Werror)
target_compile_features(mouse PRIVATE c_std_90)
target_include_directories(mouse PUBLIC include)
target_link_libraries(mouse PRIVATE ps2)

add_executable(mouse_test ${SOURCES} test/main.c)

target_compile_options(mouse_test PRIVATE -mmcu=atmega16a -pedantic -Wall -Werror)
target_compile_features(mouse_test PRIVATE c_std_90)
set_target_properties(mouse_test PROPERTIES LINK_FLAGS -mmcu=atmega16a)
target_include_directories(mouse_test PRIVATE include)
target_link_libraries(mouse_test PRIVATE ps2_mock)
