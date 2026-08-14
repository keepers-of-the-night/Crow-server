cmake_minimum_required(VERSION 3.10)
project(CrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_WIN32_WINNT=0x0A00 -DCROW_ENABLE_LOGGING)

include_directories(
    include
    openssl/include
)

add_executable(crow_server
    src/main.cpp
)

target_link_libraries(crow_server 
    F:/Alexey/C++/My_crow_server/openssl/lib/libcrypto.lib
    F:/Alexey/C++/My_crow_server/openssl/lib/libssl.lib
    ws2_32 
    mswsock
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)