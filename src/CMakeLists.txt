cmake_minimum_required(VERSION 3.17)
project(Portagens)

set(CMAKE_CXX_STANDARD 20)

add_executable(Portagens main.cpp client.cpp client.h employee.cpp employee.h lane.cpp lane.h menu.cpp menu.h systemMonitor.cpp systemMonitor.h time.cpp time.h toll.cpp toll.h vehicle.cpp vehicle.h trip.cpp trip.h highway.cpp highway.h sequentialSearch.h intervention.cpp intervention.h technician.cpp technician.h bst.h)