#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
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

Matrice viewport(float x, float y, float width, float height){
    Matrice m3 = Matrice(4, 4);
    m3.identity();
    m3.set(3,0,x+width/2.f);
    m3.set(3,1,y+height/2.f);
    m3.set(3,2,depth/2.f);
    m3.set(0, 0, width/2.f);
    m3.set(1, 1, height/2.f);
    m3.set(2, 2, depth/2.f);
    return m3;
}

point3Df perspectiveViewPort(point3Df point){
    point3Df cam = {0, 0, 3};
    Matrice m1 = Matrice(1, 3);
    m1.set(0, 0, point.x);
    m1.set(0, 1, point.y);
    m1.set(0, 2, point.z);
    m1 = m1.augmenter();
    Matrice m2 = Matrice(3, 3);
    m2.identity();
    m2 = m2.augmenter(cam);
    m1.multiply(m2);
    m1 = m1.reduire();
    m1 = m1.augmenter();
    m1.multiply(viewport(size/8, size/8, size*3/4, size*3/4));
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

Matrice setLook(vec3Df center){
    vec3Df z = substractM(eye, center);
    z.normalize();
    vec3Df x = crossProduct(up, z);
    x.normalize();
    vec3Df y = crossProduct(z, x);
    y.normalize();
    Matrice m1 = Matrice(4,4);
    m1.identity();
    Matrice m2 = Matrice(4,4);
    m2.identity();
    for (int i = 0; i < 3; i++){
        m1.set(i, 0, x[i]);
        m1.set(i, 1, y[i]);
        m1.set(i, 2, z[i]);
        m2.set(3, i, -center[i]);
    }
    m1.multiply(m2);
    return m1;
}

point3Df view(point3Df p, Matrice m){
    Matrice m1 = Matrice(1, 3);
    m1.set(0, 0, p.x);
    m1.set(0, 1, p.y);
    m1.set(0, 2, p.z);
    m1 = m1.augmenter();
    m1.multiply(m);
    point3Df p2 = {m1.get(0, 0), m1.get(0, 1), m1.get(0, 2)};
    return p2;
}

void drawFace(char* filename){
    Lecture lecture;
    vector<vector<float> > tab = lecture.readfile(filename);
    vector<point2D> line = lecture.readline(filename);
    vector<point3Df> tabTexture = lecture.readTexture(filename);
    Outils outils;
    TGAImage image(size, size, TGAImage::RGB);
    TGAImage texture;
    texture.read_tga_file("../obj/head_diffuse.tga");
    texture.flip_vertically();
    vec3Df light = vec3Df(1, 0, 1);
    light.normalize();
    vec3Df center = vec3Df(0, 0, 0);
    Matrice m = setLook(center);
    int *zbuffer = new int[size * size];
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            zbuffer[i+j*size] = numeric_limits<int>::min();
        }
    }
    for (int i = 0; i < line.size(); i+=3){
        //Sommets
        point3Df A = {tab[0][line[i].x], tab[1][line[i].x], tab[2][line[i].x]};
        point3Df B = {tab[0][line[i+1].x], tab[1][line[i+1].x],  tab[2][line[i+1].x]};
        point3Df C = {tab[0][line[i+2].x], tab[1][line[i+2].x],  tab[2][line[i+2].x]};
        //Bx - Ax; By - Ay; Bz - Az;
        vec3Df v1 = vec3Df(B.x - A.x, B.y - A.y, B.z - A.z);
        //Cx - Ax; Cy - Ay; Cz - Az;
        vec3Df v2 = vec3Df(C.x - A.x, C.y - A.y, C.z - A.z);
        //Produit vectorielle du triangle
        vec3Df crossV = crossProduct(v1, v2);
        //Normalisation des vecteurs
        crossV.normalize();
        //Calcul du cosinus
        float cos = (crossV.x * light.x + crossV.y * light.y + crossV.z * light.z) / (crossV.norm * light.norm);
        //Produit scalaire entre norme triangle et vecteur de la lumière
        float lighting = crossV.norm * light.norm * cos;
        //Transformations
        point3Df wA = perspectiveViewPort(view(A, m));
        point3Df wB = perspectiveViewPort(view(B, m));
        point3Df wC = perspectiveViewPort(view(C, m));
        if (lighting >0) {
            //Couleur des sommets
            point2Df colA = {tabTexture[line[i].y].x, tabTexture[line[i].y].y};
            point2Df colB = {tabTexture[line[i+1].y].x, tabTexture[line[i+1].y].y};
            point2Df colC = {tabTexture[line[i+2].y].x, tabTexture[line[i+2].y].y};
            point2Df pts[3] = {colA, colB, colC};
            point3Df coord[3] = {wA, wB, wC};
            outils.drawTriangle(coord, image, texture, pts, zbuffer, lighting);
        }
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }
  drawFace(argv[1]);
}
