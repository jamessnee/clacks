cmake_minimum_required (VERSION 2.6)
project (clacksdaemon)

set (clacksd_VERSION_MAJOR 1)
set (clacksd_VERSION_MINOR 0)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories (/usr/local/include)

file (GLOB generated_transports_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport-server/generated/*.c)
set (CLACKSD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/clacksd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport-server/cl_discovery.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport-server/cl_discovery_thread.c
  ${generated_transports_SRC}
  ${generated_transports_SRC})

find_package (Threads)
link_directories(/usr/local/lib)

add_executable (clacksd ${CLACKSD_SRC})

target_link_libraries (clacksd ${CMAKE_THREAD_LIBS_INIT} protobuf-c)
