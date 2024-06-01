cmake_minimum_required(VERSION 3.22)
project(Calc)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries
find_package(Boost 1.74 REQUIRED COMPONENTS iostreams system filesystem)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost libraries")
endif()

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Add the executable
add_executable(FunctionPloter main.cpp Calculator.h Calculator.cpp)

# Link Boost libraries and Gnuplot-iostream
target_link_libraries(FunctionPloter ${Boost_LIBRARIES})

