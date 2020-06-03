cmake_minimum_required(VERSION 3.10)
project(StatusBar)

set(View ../../Source/View)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
include_directories(${View})

add_executable(${PROJECT_NAME}
               Application.cpp
               ${View}/ProgressBar.cpp
               ${View}/GDIRenderer.cpp
               ${View}/DialogItem.cpp
               ${View}/MainWindow.cpp
               ${View}/StatusBar.cpp
               ${View}/Window.cpp
               ${View}/View.cpp)
