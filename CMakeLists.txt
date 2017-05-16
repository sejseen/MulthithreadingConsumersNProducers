cmake_minimum_required(VERSION 3.7)
project(MulthithreadingConsumersNProducers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(MulthithreadingConsumersNProducers ${SOURCE_FILES})