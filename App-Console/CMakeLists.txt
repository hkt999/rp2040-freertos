cmake_minimum_required(VERSION 3.14)

# Should we set app-level debugging?
# NOTE Equivalent of `#define DEBUG 1`
if(${DO_DEBUG})
    add_compile_definitions(DEBUG=1)
    message(STATUS "App-side debugging enabled for ${CONSOLE_NAME}")
else()
    message(STATUS "App-side debugging disabled for ${CONSOLE_NAME}")
endif()

# Make project data accessible to compiler
add_compile_definitions(APP_NAME="${APP_CONSOLE_NAME}")
add_compile_definitions(APP_VERSION="${APP_CONSOLE_VERSION_NUMBER}")
add_compile_definitions(BUILD_NUM=${BUILD_NUMBER})

# Include app source code file(s)
add_executable(
    ${APP_CONSOLE_NAME}
    ${APP_CONSOLE_SRC_DIRECTORY}/edit_line.cxx
    ${APP_CONSOLE_SRC_DIRECTORY}/stdio_edit.cxx
    ${APP_CONSOLE_SRC_DIRECTORY}/history.cxx
    ${APP_CONSOLE_SRC_DIRECTORY}/freertos_term.cxx
    ${APP_CONSOLE_SRC_DIRECTORY}/ministd/avl_bf.c
    ${APP_CONSOLE_SRC_DIRECTORY}/testmain.cxx
)

# Link to built libraries
target_link_libraries(${APP_CONSOLE_NAME} LINK_PUBLIC
    pico_stdlib
    FreeRTOS
)

# Enable/disable STDIO via USB and UART
pico_enable_stdio_usb(${APP_CONSOLE_NAME} 1)
pico_enable_stdio_uart(${APP_CONSOLE_NAME} 0)

# Enable extra build products
pico_add_extra_outputs(${APP_CONSOLE_NAME})
