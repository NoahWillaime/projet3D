//
// Created by willaime3u on 30/01/19.
//

#ifndef PROJET3D_MODEL_H
#define PROJET3D_MODEL_H

#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "vec3Df.hpp"
#include "tgaimage.h"

class Model {
private:
    TGAImage texture;
    TGAImage textureNormal;
    TGAImage textureSpec;
    char *filename;
    std::vector<vec3Df> tab;
    std::vector<point2D> line;
    std::vector<point3Df> tabTexture;
    std::vector<vec3Df> normalVector;
    void readline();
    void readfile();
    void readTexture();
    void readNormal();
public:
    Model();
    Model(char *filename);
    vec3Df getNormalTexture(float x, float y);
    vec3Df getTab(int i);
    point2D getLine(int i);
    point3Df getTabTexture(int i);
    vec3Df getNormalVector(int i);
    int getNbLine();
    TGAColor diffuse(point2Df textureCord);
    float specular(point2Df specCord);
};

#endif //PROJET3D_MODEL_H
