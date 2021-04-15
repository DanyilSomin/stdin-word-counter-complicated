cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT stdin-word-counter)

project(${PROJECT})

file(GLOB SOURCE *.cpp)

add_definitions(-O3 --std=c++17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries( pthread)

add_executable(${PROJECT} ${SOURCE})