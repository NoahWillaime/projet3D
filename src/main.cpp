#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include "tgaimage.h"
#include "outils.hpp"
#include "lecture.hpp"
#include "vec3Df.hpp"

using namespace std;
const int size = 800;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);
const TGAColor orange = TGAColor(255, 165, 0, 255);
const TGAColor pink = TGAColor(255, 192, 203, 255);
const TGAColor maroon = TGAColor(128, 0, 0, 255);
const TGAColor cyan = TGAColor(0, 255, 255, 255);
const TGAColor grey = TGAColor(128, 128, 128, 255);
const TGAColor darkgrey = TGAColor(47, 79, 79, 255);
const TGAColor purple = TGAColor(128, 0, 128, 255);

int world2screen(float x) {
  return (x+1)*size/2;
}

vec3Df crossProduct(point3Df v1, point3Df v2){
  float x = v1.y * v2.z - v1.z * v2.y;
  float y = v1.z * v2.x - v1.x * v2.z;
  float z = v1.x * v2.y - v1.y * v2.x;
  return vec3Df(x, y, z);
}

void drawFace(char* filename){
    Lecture lecture;
    vector<vector<float> > tab = lecture.readfile(filename);
    vector<int> line = lecture.readline(filename);
    Outils outils;
    TGAImage image(size, size, TGAImage::RGB);
    vec3Df light = vec3Df(0, 0, 1);
    int zbuffer[size][size];
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            zbuffer[i][j] = numeric_limits<int>::min();
        }
    }
    for (int i = 0; i < line.size(); i+=3){
        point2D A = {world2screen(tab[0][line[i]]), world2screen(tab[1][line[i]])};
        point2D B = {world2screen(tab[0][line[i+1]]), world2screen(tab[1][line[i+1]])};
        point2D C = {world2screen(tab[0][line[i+2]]), world2screen(tab[1][line[i+2]])};
        //Bx - Ax; By - Ay; Bz - Az;
        point3Df v1 = {tab[0][line[i+1]] - tab[0][line[i]], tab[1][line[i+1]] - tab[1][line[i]], tab[2][line[i+1]] - tab[2][line[i]]};
        //Cx - Ax; Cy - Cy; Cz - Az;
        point3Df v2 = {tab[0][line[i+2]] - tab[0][line[i]], tab[1][line[i+2]] - tab[1][line[i]], tab[2][line[i+2]] - tab[2][line[i]]};

        //Produit vectorielle du triangle
        vec3Df crossV = crossProduct(v1, v2);

        crossV.normalize();
        light.normalize();

        //cosinus
        float cos = (crossV.x * light.x + crossV.y * light.y + crossV.z * light.z) / (crossV.norm * light.norm);
        //Produit scalaire entre norme triangle et vecteur de la lumière
        float lighting = crossV.norm * light.norm * cos;
        if (lighting >0)
            outils.drawTriangle(A, B, C, image, TGAColor(lighting*255, lighting*255, lighting*255, 255));
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
}

void rasterize(point2D p0, point2D p1, TGAImage &image, TGAColor color, int ybuffer[]){
  if (p0.x >p1.x)
    swap(p0, p1);
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }
  drawFace(argv[1]);
 /* int width = size;
  TGAImage render(width, 16, TGAImage::RGB);
  int ybuffer[width];
  for (int i = 0; i<width;i++) {
    ybuffer[i] = numeric_limits<int>::min();
    render.set(i, 0, yellow);
  }*/
  //render.flip_vertically();
  //render.write_tga_file("render.tga");
}
