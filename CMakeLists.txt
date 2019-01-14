cmake_minimum_required(VERSION 3.8)
project(projet3D)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/tgaimage.cpp
        src/tgaimage.h src/outils.cpp src/outils.hpp src/lecture.cpp src/lecture.hpp)

add_executable(projet3D ${SOURCE_FILES})