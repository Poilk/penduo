cmake_minimum_required(VERSION 3.16)

project(penduo
        VERSION 0.0.1)

include(CMakePrintHelpers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Werror=return-type")

option(PENDUO_USE_NAIVE_TIMERFD "" ON)

add_subdirectory(src)