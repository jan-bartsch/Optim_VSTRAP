cmake_minimum_required(VERSION 2.8)

project(Optim_VSTRAP_CMAKE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources ../src/*.cc ../src/*.cpp *.cpp *.cc)
list(FILTER sources EXCLUDE REGEX "../src/main.cpp")

add_executable(run_tests ${sources})
