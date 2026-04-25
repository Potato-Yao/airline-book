cmake_minimum_required(VERSION 3.16)
project(airline_book)

set(CMAKE_CXX_STANDARD 17)

add_executable(airline_book main.cpp
        database/csv_handler.cpp
        database/csv_handler.h
        dao/flight.cpp
        dao/flight.h
        manager/flight_manager.cpp
        manager/flight_manager.h
        database/cell.cpp
        database/cell.h
)
