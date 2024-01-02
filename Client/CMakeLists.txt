cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Client LANGUAGES CXX)

add_subdirectory(ThirdParty/SDL)

add_executable(${PROJECT_NAME} 
    src/Main.cpp
        ../Server/src/Main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 ws2_32)