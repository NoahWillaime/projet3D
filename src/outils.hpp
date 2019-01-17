//
// Created by noahd on 14/01/2019.
//

#ifndef PROJET3D_OUTILS_HPP
#define PROJET3D_OUTILS_HPP

#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits>
#include "tgaimage.h"

struct point2D{
    int x;
    int y;
};

struct point2Df{
    float x;
    float y;
};

struct point3D{
    int x;
    int y;
    int z;
};

struct point3Df{
    float x;
    float y;
    float z;
};

class Outils {
private:
    int getMin(int *param);
    int getMax(int *param);
    std::vector<std::vector<int> > lineTriangle(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, TGAImage &image, TGAColor color);
    std::vector<point2D> boundingBox(point3Df A, point3Df B, point3Df C);
public:
    Outils();
    std::vector<std::vector<int> > drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    void drawTriangle(point3Df A, point3Df B, point3Df C, TGAImage &image, TGAColor color, int *zbuffer);
};


#endif //PROJET3D_OUTILS_HPP
