cmake_minimum_required (VERSION 2.8)

project (Zerv0r)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output/)

aux_source_directory(./src DIR_SRCS)

add_compile_options(-g -Wall)
link_libraries(pthread)

add_executable(Zerv0r ${DIR_SRCS})