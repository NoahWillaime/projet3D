//
// Created by noahd on 16/01/2019.
//

#include "vec3Df.hpp"

vec3Df::vec3Df(float Vx, float Vy, float Vz):x(Vx),y(Vy),z(Vz){
    norm = sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void vec3Df::normalize() {
    x /= norm;
    y /= norm;
    z /= norm;
}