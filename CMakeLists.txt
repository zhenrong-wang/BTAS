cmake_minimum_required(VERSION 3.15)

project(filter_uniq_ints LANGUAGES C CXX)

message("* Current build type is : ${CMAKE_BUILD_TYPE}")

add_executable(fui
        filter_uniq_ints.c
        main.cpp
)
