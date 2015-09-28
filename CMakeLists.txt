cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_definitions (-DCLACKS_DEBUG)

add_subdirectory (clacksd)
add_subdirectory (lib)
add_subdirectory (identifier-service)
