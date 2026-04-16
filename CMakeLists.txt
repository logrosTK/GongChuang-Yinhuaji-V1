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
set(CMAKE_PROJECT_NAME NEW2)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # User application code
    USER_Code/io_devices.c
    USER_Code/chassis_control.c
    USER_Code/delay_us.c
    USER_Code/position.c
    USER_Code/oledfont.c

    USER_Code/Visual_Identity/visual_identity.c
    USER_Code/tower/tower.c
    USER_Code/tower/hcan.c

    # RTOS tasks
    RTOS_APP/Main_task.c
    RTOS_APP/Chassis_task.c
    RTOS_APP/Output_task.c
    RTOS_APP/Tower_task.c

    # Driver libraries
    J4310_Drivers/bsp_can.c
    J4310_Drivers/dm_motor_ctrl.c
    J4310_Drivers/dm_motor_drv.c
    ZDT_Drivers/X_V2.c
    LD01M_Drivers/LobotServoController.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    USER_Code
    USER_Code/OLED_SoftSPI
    USER_Code/Visual_Identity
    USER_Code/tower
    RTOS_APP
    J4310_Drivers
    ZDT_Drivers
    LD01M_Drivers
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
