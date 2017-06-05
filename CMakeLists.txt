cmake_minimum_required(VERSION 3.7)
project(MulthithreadingConsumersNProducers)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
find_package(Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurses -pthread")

set(SOURCE_FILES main.cpp WorkerService.cpp WorkerService.h QueueStructure.cpp QueueStructure.h Process.h )
add_executable(MulthithreadingConsumersNProducers ${SOURCE_FILES})

target_link_libraries(MulthithreadingConsumersNProducers ${CURSES_LIBRARIES})
target_link_libraries(MulthithreadingConsumersNProducers ${CMAKE_THREAD_LIBS_INIT})