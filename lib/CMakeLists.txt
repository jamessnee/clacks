project (libclacks C)
cmake_minimum_required (VERSION 2.6)

set (LIBCLACKS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/clacks-api/clacks.c)
add_library (clacks SHARED ${LIBCLACKS_SRC})
install (TARGETS clacks DESTINATION lib)
install (FILES clacks.h DESTINATION include)
