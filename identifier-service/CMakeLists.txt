cmake_minimum_required (VERSION 2.6)
project (identifierdaemon)

set (clacksidd_VERSION_MAJOR 1)
set (clacksidd_VERSION_MINOR 0)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories (/usr/local/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src/)

set (CLACKSD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/clacksidd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/id_generator.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/id_dom_socket.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/id_tag_manager.c
  )

find_package (Threads)
find_package (LibUUID)
link_directories(/usr/local/lib)

add_executable (clacksidd ${CLACKSD_SRC})

target_link_libraries (clacksidd ${CMAKE_THREAD_LIBS_INIT} ${LIBUUID_LIBRARIES})
