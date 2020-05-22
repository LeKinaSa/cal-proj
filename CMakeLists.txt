cmake_minimum_required(VERSION 3.15.3)
project(cal_proj)

set(CMAKE_CXX_STANDARD 14)

add_executable(cal_proj src/main.cpp src/Graph.h src/MutablePriorityQueue.h src/branchAndBound.cpp src/branchAndBound.h)