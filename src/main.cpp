#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include "tgaimage.h"
#include "outils.hpp"
#include "lecture.hpp"
#include "vec3Df.hpp"
#include "matrice.h"

using namespace std;
const int size = 800;
const int depth = 255;
const vec3Df eye = vec3Df(1, 1, 3);
const vec3Df up = vec3Df(0, 1, 0);

Matrice m1 = Matrice(1, 4);
Matrice m2 = Matrice(4, 4);
Matrice vp = Matrice(4, 4);
Matrice lookat = Matrice(4, 4);

void viewport(float x, float y, float width, float height){
    vp.identity();
    vp.set(3,0,x+width/2.f);
    vp.set(3,1,y+height/2.f);
    vp.set(3,2,depth/2.f);
    vp.set(0, 0, width/2.f);
    vp.set(1, 1, height/2.f);
    vp.set(2, 2, depth/2.f);
}

point3Df perspectiveViewPort(point3Df point){
    point3Df cam = {0, 0, eye.z};
    m1.reset();
    m1.set(0, 0, point.x);
    m1.set(0, 1, point.y);
    m1.set(0, 2, point.z);
    m1.set(0, 3, 1);
    m2.reset();
    m2.identity();
    m2.augmenter(cam);
    m1.multiply(m2);
    m1.reduire();
    m1.set(0, 3, 1);
    m1.multiply(vp);
    point3Df p = {m1.get(0, 0), m1.get(0, 1), m1.get(0, 2)};
    return p;
}

vec3Df crossProduct(vec3Df v1, vec3Df v2){
  float x = v1.y * v2.z - v1.z * v2.y;
  float y = v1.z * v2.x - v1.x * v2.z;
  float z = v1.x * v2.y - v1.y * v2.x;
  return vec3Df(x, y, z);
}

vec3Df substractM(vec3Df a, vec3Df b){
    return vec3Df(a.x-b.x, a.y-b.y, a.z-b.z);
}

void setLook(vec3Df center){
    vec3Df z = substractM(eye, center);
    z.normalize();
    vec3Df x = crossProduct(up, z);
    x.normalize();
    vec3Df y = crossProduct(z, x);
    y.normalize();
    lookat.reset();
    lookat.identity();
    m2.reset();
    m2.identity();
    for (int i = 0; i < 3; i++){
        lookat.set(i, 0, x[i]);
        lookat.set(i, 1, y[i]);
        lookat.set(i, 2, z[i]);
        m2.set(3, i, -center[i]);
    }
    lookat.multiply(m2);
}

point3Df view(vec3Df p, Matrice m){
    m1.reset();
    m1.set(0, 0, p.x);
    m1.set(0, 1, p.y);
    m1.set(0, 2, p.z);
    m1.set(0, 3, 1);
    m1.multiply(m);
    point3Df p2 = {m1.get(0, 0), m1.get(0, 1), m1.get(0, 2)};
    return p2;
}

point3Df getLight(vec3Df A, vec3Df B, vec3Df C, vec3Df light){
    float cos;
    float lA, lB, lC;
    A.normalize();
    B.normalize();
    C.normalize();
    cos = (A.x * light.x + A.y * light.y + A.z * light.z) / (A.norm * light.norm);
    lA = A.norm * light.norm * cos;
    cos = (B.x * light.x + B.y * light.y + B.z * light.z) / (B.norm * light.norm);
    lB = B.norm * light.norm * cos;
    cos = (C.x * light.x + C.y * light.y + C.z * light.z) / (C.norm * light.norm);
    lC = C.norm * light.norm * cos;
    point3Df p = {lA, lB, lC};
    return p;
}

void drawFace(char* filename){
    Lecture lecture;
    vector<vec3Df> tab = lecture.readfile(filename);
    vector<point2D> line = lecture.readline(filename);
    vector<point3Df> tabTexture = lecture.readTexture(filename);
    vector<vec3Df> normalVector = lecture.readNormal(filename);
    Outils outils;
    TGAImage image(size, size, TGAImage::RGB);
    TGAImage texture;
    texture.read_tga_file("../obj/head_diffuse.tga");
    texture.flip_vertically();
    vec3Df light = vec3Df(1, -1, 1);
    light.normalize();
    vec3Df center = vec3Df(0, 0, 0);
    setLook(center);
    viewport(size/8, size/8, size*3/4, size*3/4);
    int *zbuffer = new int[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            zbuffer[i + j * size] = numeric_limits<int>::min();
        }
    }
    double time_spent;
    for (int i = 0; i < line.size(); i+=3){
        //Sommets
        vec3Df A = tab[line[i].x];
        vec3Df B = tab[line[i+1].x];
        vec3Df C = tab[line[i+2].x];
        point3Df lightVector = getLight(normalVector[line[i].x], normalVector[line[i+1].x], normalVector[line[i+2].x], light);
        //Transformations
        point3Df wA = perspectiveViewPort(view(A, lookat));
        point3Df wB = perspectiveViewPort(view(B, lookat));
        point3Df wC = perspectiveViewPort(view(C, lookat));
        //Couleur des sommets
        point2Df colA = {tabTexture[line[i].y].x, tabTexture[line[i].y].y};
        point2Df colB = {tabTexture[line[i+1].y].x, tabTexture[line[i+1].y].y};
        point2Df colC = {tabTexture[line[i+2].y].x, tabTexture[line[i+2].y].y};
        point2Df pts[3] = {colA, colB, colC};
        point3Df coord[3] = {wA, wB, wC};
        clock_t begin, end;
        begin = clock();
        outils.drawTriangle(coord, image, texture, pts, zbuffer, lightVector);
        end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    cout << time_spent << endl;
    image.flip_vertically();
    image.write_tga_file("output.tga");
    m1.delMatrice();
    m2.delMatrice();
    vp.delMatrice();
    lookat.delMatrice();
    delete[] zbuffer;
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }

  drawFace(argv[1]);

}
