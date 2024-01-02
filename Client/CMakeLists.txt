cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Client LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    src/Main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)