cmake_minimum_required(VERSION 3.16)

project(matrix VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

include_directories(
    src/display
    src/engine
)

qt_add_executable(matrix
    src/main.cpp
    src/display/window.cpp
    src/engine/project.cpp
    src/engine/layer.cpp
    src/engine/object.cpp
    src/engine/group.cpp
)

target_link_libraries(matrix PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_compile_definitions(matrix PRIVATE -DQT_DEBUG -DQT_MESSAGELOGCONTEXT)
