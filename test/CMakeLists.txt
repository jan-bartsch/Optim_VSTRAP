cmake_minimum_required(VERSION 3.10)

project(Optim_VSTRAP_CMAKE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVSTRAP_DEBUG -fno-inline --coverage")
set(USE_CUDA FALSE)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost  1.36.0 COMPONENTS log)
find_package(OpenMP REQUIRED)
find_package(CUDA)
find_package(GTest REQUIRED)

find_package(Armadillo REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} ${ARMADILLO_INCLUDE_DIRS})
file(GLOB_RECURSE sources ../src/*.cc ../src/*.cpp *.cpp)
list(FILTER sources EXCLUDE REGEX "../src/main.cpp")
add_executable(run_tests ${sources})
target_link_libraries(run_tests ${Boost_LIBRARIES} OpenMP::OpenMP_CXX ${GTEST_LIBRARIES}
    ${ARMADILLO_LIBRARIES} -lstdc++fs pthread --coverage)


