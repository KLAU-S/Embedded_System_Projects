

cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(tictactoe C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
if (VERBOSE)
    add_compile_definitions(VERBOSE)
endif()
pico_sdk_init()
add_executable(${PROJECT_NAME}
    game.hpp
    game.cpp
    main.cpp
)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(
    ${PROJECT_NAME}
    pico_stdlib
    pico_multicore
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)