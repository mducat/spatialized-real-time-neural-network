cmake_minimum_required(VERSION 3.16)

project(matrix VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

include_directories(
        src/
        src/display
        src/display/widgets
        src/display/widgets/live_analyzer
        src/workspace
        src/workspace/sinviz
        src/workspace/plot_test
        src/workspace/main
        src/workspace/empty
        src/helpers
        src/engine
        src/engine/time
        src/engine/data
        src/engine/math
        src/engine/layers
        src/engine/layers/network
        src/engine/layers/any
)

qt_add_library(mlib
        src/launcher.cpp

        # UI

        src/display/window.cpp
        src/display/widgets/live_analyzer/display.cpp
        src/display/widgets/live_analyzer/group.cpp
        src/display/widgets/live_analyzer/value.cpp
        src/display/widgets/viewer/scene.cpp
        src/display/widgets/inspector/inspector.cpp
        src/display/widgets/inspector/object_list.cpp
        src/display/widgets/generic/plot.cpp
        src/display/widgets/viewer/view.cpp
        src/display/widgets/viewer/view.hpp

        # UI - PANEL

        src/display/panel/menu.cpp
        src/display/panel/toolbar.cpp
        src/display/panel/scene_tabs.cpp
        src/display/panel/scene_tabs.hpp
        src/display/panel/layer_display.cpp

        # ENGINE

        src/engine/project.cpp
        src/engine/layer.cpp
        src/engine/layers/group.cpp
        src/engine/layers/object.cpp

        # ENGINE - NETWORK OBJ

        src/engine/layers/network/integrate_fire.cpp
        src/engine/layers/network/input_holder.cpp
        src/engine/layers/network/network_object.cpp
        src/engine/layers/network/sin.cpp

        # ENGINE - UTILS

        src/engine/time/delta.cpp
        src/engine/math/voltage_ode.cpp
        src/engine/data/dynamic_data.cpp
        src/engine/data/static_data.cpp
        src/engine/layers/network/random.cpp

)

target_link_libraries(mlib PRIVATE
        Qt6::Core
        Qt6::Widgets
)

qt_add_executable(matrix     src/workspace/main.cpp)
qt_add_executable(ui_test    src/workspace/ui_test.cpp)
qt_add_executable(plot_test  src/workspace/plot_test.cpp)
qt_add_executable(sinviz     src/workspace/sinviz.cpp)

set (_executables_list "matrix;ui_test;plot_test;sinviz")

foreach (my_entry IN LISTS _executables_list)

    message(STATUS "LINK ${my_entry}")

    target_link_libraries("${my_entry}" PRIVATE
            Qt6::Core
            Qt6::Widgets
            mlib
    )

endforeach()

target_compile_definitions(matrix PRIVATE -DQT_DEBUG -DQT_MESSAGELOGCONTEXT)
