cmake_minimum_required(VERSION 3.3)

project(Optim_VSTRAP_CMAKE LANGUAGES CXX)

set(USE_CUDA FALSE)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DOPTIM_VSTRAP_DEBUG")
find_package(Boost 1.36.0  COMPONENTS log)

find_package(OpenMP REQUIRED)
find_package(Armadillo REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} ${ARMADILLO_INCLUDE_DIRS})
    file(GLOB_RECURSE sources *.cc *.cpp)
    add_executable(${PROJECT_NAME} ${sources})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${ARMADILLO_LIBRARIES} OpenMP::OpenMP_CXX
      libpthread.so.0 librt.so -llapack -lblas -ltbb -ltbbmalloc -fopenmp)
endif()


#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS} ${ARMADILLO_INCLUDE_DIRS})
#    file(GLOB_RECURSE sources *.cc *.cpp)
#    add_executable(${PROJECT_NAME} ${sources})
#    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${ARMADILLO_LIBRARIES} OpenMP::OpenMP_CXX
#        libpthread.so.0 librt.so -llapack -lblas -ltbb -ltbbmalloc)
#endif()

