add_library(ps2_interface INTERFACE)

target_include_directories(ps2_interface INTERFACE include)

add_library(ps2 STATIC include/ps2/ps2.h src/ps2.c)

target_compile_options(ps2 PRIVATE -mmcu=${MCU} -pedantic -Wall -Werror)
target_compile_features(ps2 PRIVATE c_std_90)
target_link_libraries(ps2 PUBLIC ps2_interface)
