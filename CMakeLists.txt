cmake_minimum_required(VERSION 3.10)
project(CrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_WIN32_WINNT=0x0A00 -DCROW_ENABLE_LOGGING)

include_directories(include)

add_executable(crow_server
    src/main.cpp
)