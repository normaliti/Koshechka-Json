cmake_minimum_required(VERSION 3.0)

project(MyFirstGUI)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(PUBLIC imgui)
include_directories(PUBLIC imgui-sfml)

add_executable(executable main.cpp 
imgui/imgui.cpp 
imgui/imgui_widgets.cpp 
imgui/imgui_draw.cpp
imgui/imgui_tables.cpp
imgui-sfml/imgui-SFML.cpp 
interface/interface.cpp)

add_executable(executable_sfml_only main-sfml_only.cpp)

target_link_libraries(executable PUBLIC sfml-window sfml-graphics sfml-system GL)
target_link_libraries(executable_sfml_only PUBLIC sfml-window sfml-graphics sfml-system GL)