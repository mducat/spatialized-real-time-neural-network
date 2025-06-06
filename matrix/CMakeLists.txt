cmake_minimum_required(VERSION 3.16)

project(matrix VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebSockets)

qt_standard_project_setup()

include_directories(
        lib/byteobject

        src/
        src/display
        src/display/widgets
        src/display/widgets/live_analyzer
        src/server
        src/server/controllers
        src/workspace
        src/workspace/sinviz
        src/workspace/plot_test
        src/workspace/main
        src/workspace/empty
        src/helpers
        src/engine
        src/engine/time
        src/engine/data_inspect
        src/engine/math
        src/engine/objects
        src/engine/objects/network
        src/engine/objects/any
        src/engine/objects/data
)

qt_add_library(mlib

        lib/byteobject/ByteObject.cpp
        lib/byteobject/ByteObject.tcc

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

        # UI - NETWORK

        src/server/display_server.cpp
        src/server/display_server.hpp
        src/server/blueprint.cpp
        src/server/blueprint.hpp
        src/server/client.cpp
        src/server/client.hpp

        # UI - NETWORK - CONTROLLERS

        src/server/controllers/create.cpp
        src/server/controllers/command.cpp
        src/server/controllers/read.cpp

        # ENGINE

        src/engine/project.cpp
        src/engine/layer.cpp
        src/engine/objects/group.cpp
        src/engine/objects/object.cpp

        # ENGINE - NETWORK OBJ

        src/engine/objects/network/dataset_entry.cpp
        src/engine/objects/network/integrate_fire.cpp
        src/engine/objects/network/input_holder.cpp
        src/engine/objects/network/network_object.cpp
        src/engine/objects/network/sin.cpp
        src/engine/objects/network/random.cpp
        src/engine/objects/network/loss.cpp

        # ENGINE - DATA OBJ

        src/engine/objects/data/dataset.cpp
        src/engine/objects/data/data_object.cpp

        # ENGINE - UTILS

        src/engine/time/delta.cpp
        src/engine/math/voltage_ode.cpp
        src/engine/data_inspect/dynamic_data.cpp
        src/engine/data_inspect/static_data.cpp
        src/engine/dataset/tensor.cpp
        src/engine/dataset/loaders/mnist_loader.cpp
        src/engine/math/losses.cpp
        src/engine/objects/network/loss.hpp

        # UNSORTED

)


include(FetchContent)
FetchContent_Declare(
        cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG        v0.8.3 # <HASH or TAG>
)
FetchContent_MakeAvailable(cpptrace)


target_link_libraries(mlib PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::WebSockets
        cpptrace::cpptrace
)

qt_add_executable(matrix     src/workspace/main.cpp)
qt_add_executable(ui_test    src/workspace/ui_test.cpp)
qt_add_executable(plot_test  src/workspace/plot_test.cpp)
qt_add_executable(sinviz     src/workspace/sinviz.cpp)
qt_add_executable(mnist      src/workspace/mnist.cpp)
qt_add_executable(ws         src/workspace/ws.cpp)

set (_executables_list "matrix;ui_test;plot_test;sinviz;mnist;ws")

foreach (my_entry IN LISTS _executables_list)

    message(STATUS "LINK ${my_entry}")

    target_link_libraries("${my_entry}" PRIVATE
            Qt6::Core
            Qt6::Widgets
            Qt6::WebSockets
            mlib
    )

endforeach()

# Needed for shared library builds on windows:  copy cpptrace.dll to the same directory as the
# executable for your_target
if(WIN32)
    add_custom_command(
            TARGET mlib POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:cpptrace::cpptrace>
            $<TARGET_FILE_DIR:mlib>
    )
endif()

target_compile_definitions(matrix PRIVATE -DQT_DEBUG -DQT_MESSAGELOGCONTEXT)
