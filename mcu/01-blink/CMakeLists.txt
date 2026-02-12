cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(01_blink C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/memmap_rp2040.ld)

pico_sdk_init()

add_executable(01_blink
    main.c
)

target_link_libraries(01_blink pico_stdlib)

pico_add_extra_outputs(01_blink)
