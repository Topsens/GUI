cmake_minimum_required(VERSION 3.10)
project(Ruler)
add_executable(${PROJECT_NAME} WIN32
               Application.cpp
               ${SOURCE_CPP})