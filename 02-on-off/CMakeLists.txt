cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(02-on-off C CXX ASM)

pico_sdk_init()

add_executable(02-on-off
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
)

target_link_libraries(02-on-off
    pico_stdlib
    hardware_gpio
)

target_include_directories(02-on-off PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/led
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task
)

pico_enable_stdio_usb(02-on-off 1)
pico_enable_stdio_uart(02-on-off 0)

pico_add_extra_outputs(02-on-off)