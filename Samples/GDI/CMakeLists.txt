cmake_minimum_required(VERSION 3.10)
project(GDI)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
add_executable(${PROJECT_NAME}
               Application.cpp
               ${SOURCE_CPP})