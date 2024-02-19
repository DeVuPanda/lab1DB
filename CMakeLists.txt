cmake_minimum_required(VERSION 3.27)
project(lab1DBnew)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1DBnew main.cpp
        functions.cpp
        Client.h
        Order.h)
