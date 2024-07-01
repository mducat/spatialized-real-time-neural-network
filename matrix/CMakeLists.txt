cmake_minimum_required(VERSION 3.16)

project(matrix VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

include_directories(
        src/display
        src/widgets
        src/widgets/live_analyzer
        src/auto_init
        src/engine
        src/engine/time
        src/engine/layers
        src/engine/layers/network
        src/engine/layers/any
)

qt_add_executable(matrix
        src/main.cpp
        src/display/window.cpp
        src/display/widgets/live_analyzer/display.cpp
        src/display/widgets/live_analyzer/group.cpp
        src/display/widgets/live_analyzer/value.cpp
        src/auto_init/main.cpp
        src/engine/project.cpp
        src/engine/layer.cpp
        src/engine/layers/group.cpp
        src/engine/layers/object.cpp
        src/engine/layers/network/integrate_fire.cpp
        src/engine/layers/network/input_holder.cpp
        src/engine/layers/network/network_object.cpp
        src/engine/layers/time/delta.cpp
)

target_link_libraries(matrix PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_compile_definitions(matrix PRIVATE -DQT_DEBUG -DQT_MESSAGELOGCONTEXT)
