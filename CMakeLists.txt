cmake_minimum_required(VERSION 3.10)

project(quaternion)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-g ") #TODO: add -Wpedantic
# set(CMAKE_CXX_FLAGS "-DARCHEUS_INIT -DARCHEUS_DEINIT -g") #TODO: add -Wpedantic

add_executable(quaternion
    src/camera.cpp
    src/main.cpp
    src/material.cpp
    src/mesh.cpp
    src/shader.cpp
    src/texture.cpp
    src/window.cpp

    src/light/directional.cpp
    src/light/light.cpp
    src/light/point.cpp
    src/light/spot.cpp
)

target_include_directories(quaternion
    PUBLIC lib/include
)

target_link_libraries(quaternion PUBLIC GL GLEW glfw)