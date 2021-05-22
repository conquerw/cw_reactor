cmake_minimum_required(VERSION 3.5)

project(cw_reactor)

include_directories(../src
					../src/data
					../src/util
					../sample)
					
aux_source_directory(../src SRC)
aux_source_directory(../src/data SRC_DATA)
aux_source_directory(../src/util SRC_UTIL)
aux_source_directory(../sample SRC_DEMO)

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g -std=gnu11 -Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -std=gnu++11 -Wall")
message(STATUS "CMAKE_C_COMPILER = " ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER = " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_C_FLAGS})
message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)

add_executable(cw_reactor ${SRC} ${SRC_DATA} ${SRC_UTIL} ${SRC_DEMO})
target_link_libraries(cw_reactor pthread)
