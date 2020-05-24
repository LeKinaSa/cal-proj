cmake_minimum_required(VERSION 3.16)
project(cal_proj)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
# The line below is necessary if you are under Windows only
# Comment the line below if you are under Linux or Mac OS
# link_libraries(ws2_32 wsock32)

add_executable(cal_proj src/main.cpp src/branchAndBound.cpp src/nearestNeighbour.cpp src/parsing.cpp
        src/PosInfo.cpp src/mockMatrices.cpp src/CCTSPbenchmark.cpp src/menu.cpp lib/graphviewer.cpp lib/connection.cpp)
