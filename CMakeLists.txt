cmake_minimum_required(VERSION 3.26)
project(cpp_transport_catalogue)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=pedantic -Werror=missing-field-initializers -Wfatal-errors")
add_executable(cpp_transport_catalogue main.cpp
        transport_catalogue.h
        geo.h
        input_reader.cpp
        input_reader.h
        stat_reader.h
        stat_reader.cpp
        transport_catalogue.cpp
        tests.h)
