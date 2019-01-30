cmake_minimum_required(VERSION 3.8)
project(projet3D)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/tgaimage.cpp
        src/tgaimage.h src/vec3Df.cpp src/vec3Df.hpp src/matrice.cpp src/matrice.h src/Model.cpp src/Model.h src/our_gl.cpp src/our_gl.h)

add_executable(projet3D ${SOURCE_FILES})