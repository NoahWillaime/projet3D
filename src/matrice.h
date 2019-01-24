//
// Created by noah on 22/01/19.
//

#ifndef PROJET3D_MATRICE_H
#define PROJET3D_MATRICE_H

#include "outils.hpp"

class Matrice {
private:
    int width;
    int height;
    float **matrix;
public:
    Matrice(int width, int height);
    ~Matrice();
    void multiply(Matrice mat);
    Matrice reduire();
    Matrice augmenter(point3Df camera);
    Matrice augmenter();
    void set(int x, int y, float p);
    int get_width();
    int get_height();
    float get(int x, int y);
    void print();
};


#endif //PROJET3D_MATRICE_H
