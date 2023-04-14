cmake_minimum_required(VERSION 3.20)

# PICO_SDK_PATH env var must point to the sdk installation path
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
# We also need PICO EXTRAS library
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

project(vertical-garden-controller LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_sleep
                        hardware_rtc
)

# Enable USB serial output and disable UARTS
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
