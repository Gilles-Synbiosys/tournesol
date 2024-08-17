cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME firmwareTurret) # <-- Name your project/executable here!

include(pico_sdk_import.cmake)
include(pimoroni_pico_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files
add_executable(${NAME}
    # motor2040_quad_velocity_sequence.cpp
    # chatgptexample.cpp
    # serial_test.cpp
    serial_binary_test.cpp
    # serial_motor.cpp # <-- Add source files here!
)

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(drivers/button/button)
include(drivers/pid/pid)
include(libraries/bitmap_fonts/bitmap_fonts)
include(libraries/hershey_fonts/hershey_fonts)
include(libraries/pico_explorer/pico_explorer)
include(libraries/motor2040/motor2040)

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
    pimoroni_i2c
    pico_explorer
    motor2040
    button
    pid
    pico_stdlib
     # <-- List libraries here!
)
pico_enable_stdio_usb(firmwareTurret 1)
pico_enable_stdio_uart(firmwareTurret 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
