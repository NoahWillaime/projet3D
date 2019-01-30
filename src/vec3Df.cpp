//
// Created by noahd on 16/01/2019.
//

#include "vec3Df.hpp"
#include <iostream>

vec3Df::vec3Df(): x(0), y(0), z(0) {
}

vec3Df::vec3Df(float Vx, float Vy, float Vz):x(Vx),y(Vy),z(Vz){
    norm = sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void vec3Df::normalize() {
    x /= norm;
    y /= norm;
    z /= norm;
}

float vec3Df::scalaire(vec3Df v) {
    float cos;
    cos = (x * v.x + y * v.y + z * v.z) / (norm * v.norm);
    return (norm * v.norm * cos);
}

float vec3Df::operator[](const int i) {
    if (i == 0)
        return x;
    else if(i==1)
        return y;
    else if (i == 2)
        return z;
}