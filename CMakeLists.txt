cmake_minimum_required(VERSION 3.16)
project(cal_proj)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/graph_viewer)

add_executable(cal_proj src/main.cpp src/Graph.h src/MutablePriorityQueue.h src/branchAndBound.cpp
        src/branchAndBound.h src/nearestNeighbour.cpp src/nearestNeighbour.h src/parsing.h src/parsing.cpp
        src/VertexInfo.cpp src/VertexInfo.h src/mockMatrices.cpp src/mockMatrices.h src/CCTSPbenchmark.cpp
        src/CCTSPbenchmark.h src/menu.cpp src/menu.h src/graph_viewer/graphviewer.cpp src/graph_viewer/connection.cpp)
