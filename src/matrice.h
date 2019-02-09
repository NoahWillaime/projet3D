//
// Created by noah on 22/01/19.
//

#ifndef PROJET3D_MATRICE_H
#define PROJET3D_MATRICE_H

#include <iostream>
#include "vec3Df.hpp"

class Matrice {
private:
    int width;
    int height;
    float **matrix;
public:
    Matrice(int width, int height);
    void delMatrice();
    void reset();
    Matrice multiplyCarre(Matrice mat);
    void multiply(Matrice mat);
    void identity();
    void reduire();
    void setRow(int row, vec3Df v);
    void setCol(int col, vec3Df v);
    vec3Df getCol3Df(int col);
    void set(int x, int y, float p);
    int get_width();
    int get_height();
    float get(int x, int y);
    Matrice transpose();
    Matrice inverse();
    Matrice inverse3();
    float determinant4();
    float determinant3();
    Matrice copy();
    void print();
};


#endif //PROJET3D_MATRICE_H
