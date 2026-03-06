cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(03-adc C CXX ASM)

pico_sdk_init()

add_executable(03-adc
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adc-task/adc-task.c
)

target_link_libraries(03-adc
    pico_stdlib
    hardware_gpio
    hardware_adc
)

target_include_directories(03-adc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/led
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task
    ${CMAKE_CURRENT_SOURCE_DIR}/adc-task
)

pico_enable_stdio_usb(03-adc 1)
pico_enable_stdio_uart(03-adc 0)

pico_add_extra_outputs(03-adc)