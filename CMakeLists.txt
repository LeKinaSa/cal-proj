cmake_minimum_required(VERSION 3.16)
project(cal_proj)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
# The line below is necessary if you are under Windows only
# Comment the line below if you are under Linux or Mac OS
# link_libraries(ws2_32 wsock32)

add_executable(cal_proj src/main.cpp src/Graph.h src/MutablePriorityQueue.h src/branchAndBound.cpp
        src/branchAndBound.h src/nearestNeighbour.cpp src/nearestNeighbour.h src/parsing.h src/parsing.cpp
        src/VertexInfo.cpp src/VertexInfo.h src/mockMatrices.cpp src/mockMatrices.h src/CCTSPbenchmark.cpp
        src/CCTSPbenchmark.h src/menu.cpp src/menu.h lib/graphviewer.cpp lib/connection.cpp)
