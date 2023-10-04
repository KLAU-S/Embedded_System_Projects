cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(temp-sensor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if (VERBOSE)
  add_compile_definitions(VERBOSE)
endif()

pico_sdk_init()

add_executable(${PROJECT_NAME}
    config.hpp
    config.cpp
    core1.hpp
    core1.cpp
    debounce.hpp
    debounce.cpp
    globals.hpp
    gpio_callback.cpp
    gpio_util.hpp
    gpio_util.cpp
    i2c_util.hpp
    i2c_util.cpp
    menu_handler.hpp
    menu_handler.cpp
    util.hpp
    util.cpp
    main.cpp
)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(
    ${PROJECT_NAME}
    pico_stdlib
    pico_multicore
    hardware_i2c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)