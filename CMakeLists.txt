cmake_minimum_required(VERSION 3.15)
project(video_stream)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)

include_directories(include)

set(APPLICATION_GUI include/main_window.hpp src/main_window.cpp)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

add_executable(video_stream main.cpp
        ${APPLICATION_GUI})

target_link_libraries(video_stream Qt5::Core)
target_link_libraries(video_stream Qt5::Widgets)
target_link_libraries(video_stream Qt5::Gui)

