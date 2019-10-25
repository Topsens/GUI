cmake_minimum_required(VERSION 3.10)
project(Clouds)
set(CMAKE_CXX_STANDARD 11)

set(View ../../../Source/View)
set(GL   ../../../Source/GL)
set(glew ../../../3rdParty/glew-2.1.0)

include_directories(${View} ${GL} ${glew}/include/)
link_directories(${glew}/lib/${CMAKE_VS_PLATFORM_NAME}/)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)
add_link_options(/DYNAMICBASE "opengl32.lib" "glu32.lib" "glew32.lib" "gdiplus.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib")

add_executable(${PROJECT_NAME}
               Application.cpp
               Scene.cpp

               ${View}/Dialog.cpp
               ${View}/DialogItem.cpp
               ${View}/GDIRenderer.cpp
               ${View}/MainWindow.cpp
               ${View}/View.cpp
               ${View}/Window.cpp

               ${GL}/GLCamera.cpp
               ${GL}/GLCloud.cpp
               ${GL}/GLLight.cpp
               ${GL}/GLMaterial.cpp
               ${GL}/GLScene.cpp
               ${GL}/GLShape.cpp
               ${GL}/GLTexture.cpp
               ${GL}/GLWindow.cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/${glew}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")