cmake_minimum_required(VERSION 2.8.3)
project(g_err_macros_tests)

# Search for glib with pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.32.1)

# Compiler options for GCC/Clang
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -ansi")

# Include folders
include_directories(${GLIB_INCLUDE_DIRS} ..)

# Add test executable
add_executable(tests tests.c)

# Specify dependency on GLib library
target_link_libraries(tests ${GLIB_LDFLAGS})
