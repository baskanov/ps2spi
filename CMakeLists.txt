cmake_minimum_required(VERSION 3.1)

if(${CMAKE_VERSION} VERSION_LESS 3.10)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.10)
endif()

set(CMAKE_TOOLCHAIN_FILE cmake/avr.cmake)

project(Ps2Spi VERSION 0.1.0 LANGUAGES C)

enable_testing()

set(MCU atmega8a)
set(TEST_MCU atmega16a)

add_library(ps2spi_interface INTERFACE)

target_include_directories(ps2spi_interface INTERFACE include)

add_subdirectory(src/keyboard)
add_subdirectory(src/mouse)
add_subdirectory(src/ps2)
add_subdirectory(src/ps2_mock)

add_executable(ps2spi include/ps2spi/ps2_keyboard_codes_enum.h include/ps2spi/ps2_keyboard_codes.def src/main.c)

target_compile_options(ps2spi PRIVATE -mmcu=${MCU} -pedantic -Wall -Werror)
target_compile_features(ps2spi PRIVATE c_std_90)
set_target_properties(ps2spi PROPERTIES LINK_FLAGS -mmcu=${MCU})
target_link_libraries(ps2spi PUBLIC ps2spi_interface PRIVATE keyboard mouse)
