//
// Created by noah on 22/01/19.
//

#include "matrice.h"

Matrice::Matrice(int width, int height):width(width), height(height) {
    matrix = new float*[height];
    for (int i = 0; i < height; i++){
        matrix[i] = new float[width];
        for (int j = 0; j < width; j++)
            matrix[i][j] = 0;
    }
}

Matrice::~Matrice() {
    for (int i = 0; i < height; i++){
        delete matrix[i];
    }
}

int Matrice::get_width() { return width; }
int Matrice::get_height() { return height; }
float Matrice::get(int x, int y) { return matrix[y][x]; }

void Matrice::set(int x, int y, float p) {
    if (x >= 0 && x < width && y >=0 && y < height){
        matrix[y][x] = p;
    }
}

Matrice Matrice::augmenter(point3Df camera) {
    Matrice nMatrix = Matrice(width+1, height+1);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++)
            nMatrix.set(j, i, matrix[i][j]);
    }
    (camera.x != 0)?nMatrix.set(0, height, -1 / camera.x):nMatrix.set(0, height, 0);
    (camera.y != 0)?nMatrix.set(1, height, -1 / camera.y):nMatrix.set(1, height, 0);
    (camera.z != 0)?nMatrix.set(2, height, -1 / camera.z):nMatrix.set(2, height, 0);
    nMatrix.set(height, 3, 1);
    return nMatrix;
}

Matrice Matrice::augmenter() {
    Matrice nMatrix = Matrice(width, height+1);
    for (int i = 0; i < height; i++)
        nMatrix.set(0, i, matrix[i][0]);
    nMatrix.set(0, height, 1);
    return nMatrix;
}

void Matrice::multiply(Matrice mat) {
    if (height == mat.get_width()){
        float result;
        for (int i = 0; i < mat.get_height(); i++){
            result = 0;
            for (int j = 0; j < mat.get_width(); j++){
                result += matrix[j][0] * mat.get(j, i);
            }
            matrix[i][0] = result;
        }
    }
}

Matrice Matrice::reduire() {
    Matrice nMatrix = Matrice(width, height-1);
    nMatrix.set(0, 0, matrix[0][0] / matrix[3][0]);
    nMatrix.set(0, 1, matrix[1][0] / matrix[3][0]);
    nMatrix.set(0, 2, matrix[2][0] / matrix[3][0]);
    return nMatrix;
}

void Matrice::print() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}