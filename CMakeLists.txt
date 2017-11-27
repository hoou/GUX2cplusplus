cmake_minimum_required(VERSION 3.8)
project(GUX2cplusplus)

set(CMAKE_CXX_STANDARD 11)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

set(SOURCE_FILES main.cpp Application.cpp Application.h Gui.cpp Gui.h GameLogic.cpp GameLogic.h)
add_executable(GUX2cplusplus ${SOURCE_FILES})

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(GUX2cplusplus ${GTK3_LIBRARIES})