cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Server LANGUAGES CXX)

file(GLOB_RECURSE src
    src/Main.cpp
    src/UDPSocket.cpp
    src/UDPSocket.hpp
    src/Game.cpp
    src/Game.hpp
)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)