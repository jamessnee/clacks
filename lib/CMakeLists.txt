project (libclacks C)
cmake_minimum_required (VERSION 2.6)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories (/usr/local/include)

file (GLOB generated_transports_client_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport-client/generated/*.c)
set (LIBCLACKS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/clacks-api/clacks.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/clacks-api/id_client.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport-client/cl_transport_domsock_client.c
  ${generated_transports_client_SRC}
  )
add_library (clacks SHARED ${LIBCLACKS_SRC})

link_directories(/usr/local/lib)
target_link_libraries (clacks ${CMAKE_THREAD_LIBS_INIT} protobuf-c)

install (TARGETS clacks DESTINATION lib)
install (FILES clacks.h DESTINATION include)
