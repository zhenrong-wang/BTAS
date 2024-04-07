cmake_minimum_required(VERSION 3.15)

project(filter_uniq_ints LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

message("* Current build type is : ${CMAKE_BUILD_TYPE}")

add_executable(fui
        filter_uniq_ints.c
        filter_uniq_ints_cpp.cpp
        main.cpp
)
