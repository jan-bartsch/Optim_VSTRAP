cmake_minimum_required(VERSION 3.10)

project(Optim_VSTRAP_CMAKE)


SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/lib")

SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/googletest/lib")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/googletest/googletest/include/")


SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/tbb/include/")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/Spaces/Promotion/codes/lapack-3.8.0/LAPACKE/include/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/Spaces/Promotion/codes/BLAS-3.8.0/")


set(ARMADILLO_INCLUDE_DIR "/home/bartsch/Spaces/Promotion/codes/armadillo-8.500.1/include")
set(ARMADILLO_LIBRARY "/home/bartsch/Spaces/Promotion/codes/armadillo-8.500.1/libarmadillo.so")
set(ARMADILLO_INCLUDE_DIRS "/home/bartsch/Spaces/Promotion/codes/armadillo-8.500.1/include")
set(ARMADILLO_LIBRARIES "/home/bartsch/Spaces/Promotion/codes/armadillo-8.500.1/libarmadillo.so")

add_subdirectory(src)
#add_subdirectory(test)
