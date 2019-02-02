//
// Created by noahd on 16/01/2019.
//

#ifndef PROJET3D_VEC3DF_HPP
#define PROJET3D_VEC3DF_HPP

#include <cmath>
struct point2D{
    int x;
    int y;
};

struct point2Df{
    float x;
    float y;
};

struct point3Df{
    float x;
    float y;
    float z;
};
class vec3Df {
public:
    float x;
    float y;
    float z;
    float norm;
    vec3Df();
    vec3Df(float x1, float y1, float z1);
    void normalize();
    float operator[](const int i);
    float scalaire(vec3Df v);
    vec3Df mult(float num);
    vec3Df soustraction(vec3Df v);
};


#endif //PROJET3D_VEC3DF_HPP
