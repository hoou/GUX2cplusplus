cmake_minimum_required(VERSION 3.8)
project(piskvorky)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

set(SOURCE_FILES main.cpp Application.cpp Application.h Gui.cpp Gui.h GameLogic.cpp GameLogic.h Cell.cpp Cell.h Player.cpp Player.h)
add_executable(piskvorky ${SOURCE_FILES})

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(piskvorky ${GTK3_LIBRARIES})