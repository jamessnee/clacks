cmake_minimum_required (VERSION 2.6)
project (clacksdaemon)

set (clacksd_VERSION_MAJOR 1)
set (clacksd_VERSION_MINOR 0)
set (CLACKSD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/clacksd.c)

find_package (Threads)

add_executable (clacksd ${CLACKSD_SRC})

target_link_libraries (clacksd ${CMAKE_THREAD_LIBS_INIT})
