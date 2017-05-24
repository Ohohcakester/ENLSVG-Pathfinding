cmake_minimum_required(VERSION 2.8)
project(ENLSVG-Pathfinding)

# Visual Studio 2015 defaults to C++11
if (NOT WIN32)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src "${SRC_DIR}/*.*")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Pathfinding")

#add_executable(PathfindingTest ${src})
add_library(Pathfinding ${src})
target_include_directories(Pathfinding INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)