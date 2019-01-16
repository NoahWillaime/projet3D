//
// Created by noahd on 16/01/2019.
//

#ifndef PROJET3D_VEC3DF_HPP
#define PROJET3D_VEC3DF_HPP

#include <cmath>

class vec3Df {
public:
    float x;
    float y;
    float z;
    float norm;

    vec3Df(float x1, float y1, float z1);
    void normalize();
};


#endif //PROJET3D_VEC3DF_HPP
