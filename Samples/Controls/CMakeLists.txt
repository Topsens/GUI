cmake_minimum_required(VERSION 3.10)
project(Controls)

set(View ../../Source/View)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
include_directories(${View})
add_executable(${PROJECT_NAME}
               Application.cpp
               Controls.rc
               ${View}/ComboBox.cpp
               ${View}/CheckBox.cpp
               ${View}/Dialog.cpp
               ${View}/DialogItem.cpp
               ${View}/ProgressBar.cpp
               ${View}/StatusBar.cpp
               ${View}/View.cpp)