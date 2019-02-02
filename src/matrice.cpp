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

void Matrice::delMatrice() {
    for (int i = 0; i < height; i++) {
        delete matrix[i];
    }
    delete[] matrix;
}

void Matrice::reset() {
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++)
            matrix[i][j] = 0;
    }
}

int Matrice::get_width() { return width; }
int Matrice::get_height() { return height; }
float Matrice::get(int x, int y) { return matrix[y][x]; }

void Matrice::identity() {
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (i == j)
                matrix[i][j] = 1;
        }
    }
}

void Matrice::set(int x, int y, float p) {
    if (x >= 0 && x < width && y >=0 && y < height){
        matrix[y][x] = p;
    }
}

void Matrice::augmenter(point3Df camera) {
    if (camera.z != 0)
        matrix[3][2] = -1 / camera.z;
    else
        matrix[3][2] = 0;
    matrix[3][3] = 1;
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

void Matrice::reduire() {
    matrix[0][0] /= matrix[3][0];
    matrix[1][0] /= matrix[3][0];
    matrix[2][0] /= matrix[3][0];
}

Matrice Matrice::transpose() {
    Matrice temp(width, height);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            temp.set(i, j, matrix[i][j]);
        }
    }
    return temp;
}

Matrice Matrice::inverse() {
    Matrice cof(4, 4);
    Matrice temp(3, 3);
    float det = determinant4();
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            for (int u = 0; u < 3; u++){
                for (int v = 0; v < 3; v++){
                    temp.set(v, u, matrix[(u >= i)?(u+1):(u)][(v >= j)?(v+1):(v)]);
                }
            }
            cof.set(j, i, pow(-1, i+j+2)*temp.determinant3());
        }
    }
    Matrice comatrice = cof.transpose();
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            comatrice.set(j, i, comatrice.get(j, i)/det);
        }
    }
    cof.delMatrice();
    temp.delMatrice();
    return comatrice;
}

float Matrice::determinant4() {
    Matrice m1(3, 3);
    Matrice m2(3, 3);
    Matrice m3(3, 3);
    Matrice m4(3, 3);
    for (int i = 1; i < 4; i++){
        for (int j = 0; j < 3; j++){
            if (j >= 0)
                m1.set(j, i-1, matrix[i][j+1]);
            if (j >= 1)
                m2.set(j, i-1, matrix[i][j+1]);
            else
                m2.set(j, i-1, matrix[i][j]);
            if (j >= 2)
                m3.set(j, i-1, matrix[i][j+1]);
            else
                m3.set(j, i-1, matrix[i][j]);
            m4.set(j, i-1, matrix[i][j]);
        }
    }
    float det = matrix[0][0] * m1.determinant3() - matrix[0][1] * m2.determinant3() + matrix[0][2] * m3.determinant3() - matrix[0][3] * m4.determinant3();
    m1.delMatrice();
    m2.delMatrice();
    m3.delMatrice();
    m4.delMatrice();
    return det;
}

float Matrice::determinant3() {
    float row1, row2, row3;
    row1 = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    row2 = matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
    row3 = matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return row1 - row2 + row3;
}

void Matrice::print() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}