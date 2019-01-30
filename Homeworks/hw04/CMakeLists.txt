cmake_minimum_required(VERSION 3.3)
project(hw4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    ingredient.cpp
    ingredient.h
    kitchen.cpp
    kitchen.h
    main.cpp
    recipe.cpp
    recipe.h)

add_executable(hw4 ${SOURCE_FILES})