#Minimum version supporting CMAKE_CXX_STANDARD
cmake_minimum_required(VERSION 3.1.0)

#Alternative minimum version for compiler-agnostic Interprocess optimisation
#Not used since Ubuntu Xenial has CMake 3.5.4, but Bionic has 3.10.2
#and should use this instead. Credit to https://stackoverflow.com/a/47370726
#UNTESTED!
#cmake_minimum_required(VERSION 3.9.4)

#include(CheckIPOSupported)
#check_ipo_supported(RESULT ipo_supported OUTPUT error)
#if (ipo_supported)
#Ideally this should only be used for release builds - how to check?
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
#else()
#message(WARNING "IPO is not supported: ${error}")
#endif()


project (submission CXX)

#Need C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Bring the headers into the project
include_directories(submission)

#Can manually add the sources using the set command: but it's tedious, so
#set(SOURCES src/main.cpp src/farm.cpp src/task_brute.cpp src/task_create.cpp src/ender.cpp)

#Instead, add the whole folder by wildcard:
file(GLOB SOURCES "src/*.cpp")

#needed for std::thread
find_package (Threads REQUIRED)

add_executable(submission ${SOURCES})

#needed for std::thread
target_link_libraries (submission ${CMAKE_THREAD_LIBS_INIT})

#Set to ON to use OpenMP
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP REQUIRED)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()
ENDIF()
