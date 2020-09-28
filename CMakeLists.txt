cmake_minimum_required(VERSION 3.3)

project(Optim_VSTRAP_CMAKE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)

message("INFO: CMAKE_SYSTEM IS ${CMAKE_SYSTEM}")

if(("${CMAKE_SYSTEM}" STREQUAL "Linux-4.12.14-lp151.28.52-default") OR ("${CMAKE_SYSTEM}" STREQUAL "Linux-4.12.14-lp151.28.59-default")
        OR ("${CMAKE_SYSTEM}" STREQUAL "Linux-4.12.14-lp151.28.67-default"))

message("INFO: some libraries are included manually")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/include/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/boost_1_72_0/boost/builddirectory/lib/")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/tbb/include/")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/Spaces/Promotion/codes/lapack-3.8.0/LAPACKE/include/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/Spaces/Promotion/codes/BLAS-3.8.0/")

SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/bartsch/include/googletest/lib/")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/bartsch/include/googletest/googletest/include/")

set(ARMADILLO_INCLUDE_DIR "/home/bartsch/include/codes/armadillo-9.900.2/include/")
set(ARMADILLO_LIBRARY "/home/bartsch/include/codes/armadillo-9.900.2/libarmadillo.so")
set(ARMADILLO_INCLUDE_DIRS "/home/bartsch/include/codes/armadillo-9.900.2/include/")
set(ARMADILLO_LIBRARIES "/home/bartsch/include/codes/armadillo-9.900.2/libarmadillo.so")

message("INFO: CMAKE_INCLUDE_PATH IS ${CMAKE_INCLUDE_PATH}")


else()

message("INFO: all libraries are installed as root")

endif()

add_subdirectory(src)
#add_subdirectory(test)
#add_subdirectory(vldn)



