cmake_minimum_required(VERSION 3.15)
project(video_stream)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)

set(APPLICATION_GUI src/main_window.h src/main_window.cpp)
set(VIDEO_PLAYER src/video_player.h src/video_player.cpp)
set(GAUSSIAN_BLUR src/video_widget/gaussian_blur.h src/video_widget/gaussian_blur.cpp)
set(EMITTER_SURFACE src/video_widget/frame_emitter.h src/video_widget/frame_emitter.cpp)
set(OUTPUT_SURFACE src/video_widget/output_video_surface.h src/video_widget/output_video_surface.cpp)
set(SOBEL_OPERATOR src/video_widget/sobel_operator.h src/video_widget/sobel_operator.cpp)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

add_executable(video_stream
        main.cpp
        ${APPLICATION_GUI}
        ${VIDEO_PLAYER}
        ${EMITTER_SURFACE}
        ${GAUSSIAN_BLUR}
        ${SOBEL_OPERATOR}
        ${OUTPUT_SURFACE})


target_link_libraries(video_stream Qt5::Core)
target_link_libraries(video_stream Qt5::Widgets)
target_link_libraries(video_stream Qt5::Gui)
target_link_libraries(video_stream Qt5::Multimedia)
target_link_libraries(video_stream Qt5::MultimediaWidgets)
