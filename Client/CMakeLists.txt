cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Client LANGUAGES CXX)

add_subdirectory(ThirdParty/Blaze)

add_executable(${PROJECT_NAME}
    src/Main.cpp
    src/Pong.cpp
    src/Pong.hpp
    src/Cube.cpp
    src/Cube.hpp
    src/Socket/Socket.cpp
    src/Socket/Socket.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

add_custom_target(copy_game_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying resource files..."
)
add_dependencies(${PROJECT_NAME} copy_game_assets)