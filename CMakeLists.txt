cmake_minimum_required(VERSION 3.0.2)
project(ENLSVG-Pathfinding)

# Visual Studio 2015 defaults to C++11
if (NOT WIN32)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
endif()

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB src "${SRC_DIR}/*.*")

add_executable(Pathfinding ${src})