cmake_minimum_required(VERSION 3.28)
project(library-management-system)

set(CMAKE_CXX_STANDARD 98)

add_executable(server Server.cpp
        main.cpp
        Client.h
        interface.cpp
        BookManagement.cpp)
