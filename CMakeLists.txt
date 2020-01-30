cmake_minimum_required(VERSION 3.10)

project(Optim_VSTRAP_CMAKE)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/lib")

SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/googletest/lib")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/googletest/googletest/include/")

add_subdirectory(test)
add_subdirectory(src)
