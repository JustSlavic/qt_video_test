cmake_minimum_required(VERSION 3.15)
project(video_stream)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)

set(APPLICATION_GUI src/main_window.h src/main_window.cpp)
set(VIDEO_PLAYER src/video_player.h src/video_player.cpp)
set(GAUSSIAN_BLUR src/video_widget/gaussian_blur.h src/video_widget/gaussian_blur.cpp)
set(VIDEO_WIDGETS src/video_widget/output_video_surface.h src/video_widget/output_video_surface.cpp)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

add_executable(video_stream main.cpp
        ${APPLICATION_GUI} ${VIDEO_PLAYER} ${GAUSSIAN_BLUR}
        ${VIDEO_WIDGETS})

target_link_libraries(video_stream Qt5::Core)
target_link_libraries(video_stream Qt5::Widgets)
target_link_libraries(video_stream Qt5::Gui)
target_link_libraries(video_stream Qt5::Multimedia)
target_link_libraries(video_stream Qt5::MultimediaWidgets)
