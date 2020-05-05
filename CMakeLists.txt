cmake_minimum_required(VERSION 3.8)

project(media)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*/*.cpp src/*/*/*.cpp)
add_executable(media ${SOURCE_FILES})
target_compile_options(media PUBLIC -std=c++17)