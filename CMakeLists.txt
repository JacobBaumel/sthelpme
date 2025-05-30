cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME CE_tinyusb_test)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_library(tinyusb OBJECT
        lib/tinyusb/src/tusb.c
        lib/tinyusb/src/common/tusb_fifo.c
        lib/tinyusb/src/device/usbd.c
        lib/tinyusb/src/device/usbd_control.c
        lib/tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c
        lib/tinyusb/src/portable/st/typec/typec_stm32.c
        lib/tinyusb/src/typec/usbc.c
        lib/tinyusb/src/class/cdc/cdc_device.c
)

target_include_directories(tinyusb
        PUBLIC
        lib/tinyusb/src
        PRIVATE
        Drivers/CMSIS/Include
        Drivers/CMSIS/Device/ST/STM32H5xx/Include
        Drivers/STM32H5xx_HAL_Driver/Inc
)

target_compile_definitions(tinyusb PRIVATE
        -DSTM32H533xx
        -DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/Core/Inc/tusb/tusb_config.h")

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Core/Src/usb_descriptors.c)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined symbols
        -DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/Core/Inc/tusb/tusb_config.h"
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx
        tinyusb
        # Add user defined libraries
)
