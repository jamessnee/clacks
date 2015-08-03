cmake_minimum_required (VERSION 2.6)
project (clacks)

# Version Numbers
set (clacks_VERSION_MAJOR 1)
set (clacsk_VERSION_MINOR 0)

find_package (Threads)

add_executable (clacksd clacksd/clacksd.c)

target_link_libraries (clacksd ${CMAKE_THREAD_LIBS_INIT})
