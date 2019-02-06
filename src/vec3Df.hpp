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

    point3Df():x(0), y(0), z(0){}

    point3Df(float cx, float cy, float cz) : x(cx), y(cy), z(cz){}

    point3Df operator/(const float d){
        x/=d;
        y/=d;
        z/=d;
        return point3Df(x, y, z);
    }
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
