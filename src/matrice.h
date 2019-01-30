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
    void multiply(Matrice mat);
    void identity();
    void reduire();
    void augmenter(point3Df camera);
    void set(int x, int y, float p);
    int get_width();
    int get_height();
    float get(int x, int y);
    void print();
};


#endif //PROJET3D_MATRICE_H
