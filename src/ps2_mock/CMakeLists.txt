add_library(ps2_mock STATIC include/ps2_mock/ps2_mock.h src/ps2_mock.c)

target_compile_options(ps2_mock PRIVATE -mmcu=atmega8a -g -Og -pedantic -Wall -Werror)
target_compile_features(ps2_mock PRIVATE c_std_90)
target_include_directories(ps2_mock PUBLIC include)
target_link_libraries(ps2_mock PUBLIC ps2_interface)
