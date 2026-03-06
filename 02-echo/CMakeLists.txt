cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(02-echo C CXX ASM)

pico_sdk_init()

add_executable(02-echo
    main.c
)

target_link_libraries(02-echo
    pico_stdlib
)

pico_enable_stdio_usb(02-echo 1)
pico_enable_stdio_uart(02-echo 0)

pico_add_extra_outputs(02-echo)