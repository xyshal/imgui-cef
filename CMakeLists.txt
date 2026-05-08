cmake_minimum_required(VERSION 3.25)
project(imgui-cef-vnc-prototype)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/freetype.cmake)
include(cmake/glfw.cmake)
include(cmake/imgui.cmake)
include(cmake/cef.cmake)

add_subdirectory(src)
