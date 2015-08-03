project (libclacks C)
cmake_minimum_required (VERSION 2.6)

set (LIBCLACKS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/clacks-api/clacks.c)
add_library (libclacks SHARED ${LIBCLACKS_SRC})
install (TARGETS libclacks DESTINATION lib)
install (FILES libclacks.h DESTINATION include)
