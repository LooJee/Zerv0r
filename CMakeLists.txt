cmake_minimum_required (VERSION 2.8)

project (Zerv0r)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output/)

aux_source_directory(./src DIR_SRCS)
aux_source_directory(./3rdparty/cJSON/src CJSON)
aux_source_directory(./3rdparty/cThreadPool/src CTHREADPOOL)

add_compile_options(-Wall -std=c99)
link_libraries(pthread)

add_executable(Zerv0r ${DIR_SRCS} ${CJSON} ${CTHREADPOOL})