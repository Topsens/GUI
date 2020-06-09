cmake_minimum_required(VERSION 3.10)
project(SubWindow)

set(VIEW ../../Source/View)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
include_directories(${VIEW})
add_executable(${PROJECT_NAME}
               Application.cpp
               SubWindow.cpp
               ${VIEW}/GDIRenderer.cpp
               ${VIEW}/DialogItem.cpp
               ${VIEW}/MainWindow.cpp
               ${VIEW}/Window.cpp
               ${VIEW}/View.cpp)