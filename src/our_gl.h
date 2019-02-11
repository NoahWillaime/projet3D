//
// Created by willaime3u on 30/01/19.
//

#ifndef PROJET3D_OUR_GL_H
#define PROJET3D_OUR_GL_H

#include <limits>
#include <vector>
#include "vec3Df.hpp"
#include "matrice.h"
#include "tgaimage.h"

extern Matrice vp;
extern Matrice lookat;
extern Matrice projection;

struct IShader {
    //virtual ~IShader();
    virtual vec3Df vertex(int i, int j) = 0;
    virtual bool fragment(point3Df barCor, TGAColor &color) = 0;
};


void get_viewport(float x, float y, float width, float height);
void get_perspective(vec3Df eye, vec3Df center);
vec3Df viewport(vec3Df point);
point4Df perspective(point3Df point);
void setLook(vec3Df eye, vec3Df center, vec3Df up);
point3Df view(vec3Df p);
point3Df getLight(vec3Df A, vec3Df B, vec3Df C, vec3Df light);
void drawTriangle(vec3Df *coords, TGAImage &image, int *zbuffer, IShader &shader);

#endif //PROJET3D_OUR_GL_H
