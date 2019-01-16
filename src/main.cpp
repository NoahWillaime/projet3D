#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include "tgaimage.h"
#include "outils.hpp"
#include "lecture.hpp"


using namespace std;
const int size = 500;


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

point3Df crossProduct(point3Df v1, point3Df v2){
  point3Df crossV;
  crossV.x = v1.y * v2.z - v1.y * v2.z;
  crossV.y = v1.z * v2.x - v1.x * v2.z;
  crossV.z = v1.x * v2.y - v1.y * v2.x;
  return crossV;
}

void drawFace(char* filename){
  Lecture lecture;
  vector<vector<float> > tab = lecture.readfile(filename);
  vector<int> line = lecture.readline(filename);
  Outils outils;
  TGAImage image(size, size, TGAImage::RGB);
  point3Df light = {0, 0, 0.8};
  for (int i = 0; i < line.size(); i+=3){
    int Ax = world2screen(tab[0][line[i]]);
    int Ay = world2screen(tab[1][line[i]]);
    int Bx = world2screen(tab[0][line[i+1]]);
    int By = world2screen(tab[1][line[i+1]]);
    int Cx = world2screen(tab[0][line[i+2]]);
    int Cy = world2screen(tab[1][line[i+2]]);
    //Bx - Ax; By - Ay; Bz - Az;
    point3Df v1 = {tab[0][line[i+1]] - tab[0][line[i]], tab[1][line[i+1]] - tab[1][line[i]], tab[2][line[i+1]] - tab[2][line[i]]};
    //Cx - Ax; Cy - Cy; Cz - Az;
    point3Df v2 = {tab[0][line[i+2]] - tab[0][line[i]], tab[1][line[i+2]] - tab[1][line[i]], tab[2][line[i+2]] - tab[2][line[i]]};
    //Produit vectorielle du triangle
    point3Df crossV = crossProduct(v1, v2);
    float norm = sqrtf(crossV.x*crossV.x + crossV.y*crossV.y + crossV.z*crossV.z);
    crossV.x /= norm;
    crossV.y /= norm;
    crossV.z /= norm;
    //Produit scalaire entre norme triangle et vecteur de la lumière
    float lightning = light.x * crossV.x + light.y * crossV.y + light.z * crossV.z;
    if (lightning >0)
      outils.drawTriangle(Ax, Ay, Bx, By, Cx, Cy, image, TGAColor(lightning*255, lightning*255, lightning*255, 255));
  }
  image.flip_vertically();
  image.write_tga_file("output.tga");
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }
  //drawFace(argv[1]);
  int width = size;
  TGAImage render(width, 16, TGAImage::RGB);
  int ybuffer[width];
  for (int i = 0; i<width;i++) {
    ybuffer[i] = numeric_limits<int>::min();
    render.set(i, 0, yellow);
  }
  //render.flip_vertically();
  render.write_tga_file("render.tga");
}
