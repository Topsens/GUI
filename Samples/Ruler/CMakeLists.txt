cmake_minimum_required(VERSION 3.10)
project(Ruler)
set(View ../../Source/View)
add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
include_directories(${View} ../../Source/Common)
add_executable(${PROJECT_NAME}
               Application.cpp
               ${View}/View.cpp
               ${View}/Window.cpp
               ${View}/MainWindow.cpp
               ${View}/DialogItem.cpp
               ${View}/D2DRenderer.cpp
               ${View}/DDCRenderer.cpp)