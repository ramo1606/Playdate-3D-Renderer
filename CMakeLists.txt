cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 90)

# Find Playdate SDK
set(ENVSDK $ENV{PLAYDATE_SDK_PATH})
if (NOT ${ENVSDK} STREQUAL "")
    # Convert path from Windows
    file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
    execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
if (NOT EXISTS ${SDK})
    message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
    return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME 3D_Renderer)
set(PLAYDATE_GAME_DEVICE 3D_Renderer_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/lib)

# Explicitly list header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/include/display.h
)

# Explicitly list source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/src/display.c
)

# Main file
set(MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Source/src/main.c)

# Create source groups (filters for Visual Studio)
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("" FILES ${MAIN_FILE})

if (TOOLCHAIN STREQUAL "armgcc")
    add_executable(${PLAYDATE_GAME_DEVICE} 
        ${HEADER_FILES} 
        ${SOURCE_FILES} 
        ${MAIN_FILE}
        ${SDK}/C_API/buildsupport/setup.c
    )
else()
    add_library(${PLAYDATE_GAME_NAME} SHARED 
        ${HEADER_FILES} 
        ${SOURCE_FILES} 
        ${MAIN_FILE}
    )
endif()

# Include Playdate game support
include(${SDK}/C_API/buildsupport/playdate_game.cmake)

# Additional compile definitions or options (if needed)
# target_compile_definitions(${PLAYDATE_GAME_NAME} PRIVATE SOME_DEFINITION=1)
# target_compile_options(${PLAYDATE_GAME_NAME} PRIVATE -Wall -Wextra)

#cmake .. -G "NMake Makefiles" --toolchain=C:/Users/Ramo/Documents/PlaydateSDK/C_API/buildsupport/arm.cmake -DCMAKE_BUILD_TYPE=Release