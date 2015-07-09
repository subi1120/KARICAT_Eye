cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(KARIKAT_Eye)

find_package(OpenCV REQUIRED)
include_directories(/usr/include/ffmpeg /usr/local/include ../include)
add_executable (KARIKAT_Eye src/CameraSensor.cpp src/DetectingMode.cpp src/Mode.cpp src/ModeManager.cpp src/ReadyMode.cpp src/TrackingMode.cpp src/MPEGCoder.cpp src/UDPTranceiver.cpp src/VideoModule.cpp src/XtionProCamera.cpp src/tracker.cpp src/RANSAC_Tracker.cpp src/RANSAC_plane.cpp src/KARIKAT_Eye.cpp)
#add_executable (KARIKAT_Eye src/*.cpp)
find_library(MyLIB NAMES /usr/lib /usr/local/lib)
target_link_libraries (KARIKAT_Eye avcodec_ avutil_ avformat_ avfilter_  avdevice_ swresample_ swscale_ urg_cpp rt z kai2 OpenNI2  ${OpenCV_LIBS})

