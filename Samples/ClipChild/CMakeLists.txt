cmake_minimum_required(VERSION 3.10)
project(ClipChild)
add_executable(${PROJECT_NAME} WIN32
               Application.cpp
               SubWindow.cpp
               ${SOURCE_CPP})