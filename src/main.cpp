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

float world2screen(float x) {
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
    vector<point2D> line = lecture.readline(filename);
    vector<point3Df> tabTexture = lecture.readTexture(filename);
    Outils outils;
    TGAImage image(size, size, TGAImage::RGB);
    vec3Df light = vec3Df(0, 0, 1);
    TGAImage texture;
    texture.read_tga_file("../obj/head_diffuse.tga");
    texture.flip_vertically();
    int *zbuffer = new int[size * size];
    int sizeT = texture.get_width();
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
        //Couleur des sommets
        point2Df colA = {tabTexture[line[i].y].x, tabTexture[line[i].y].y};
        point2Df colB = {tabTexture[line[i+1].y].x, tabTexture[line[i+1].y].y};
        point2Df colC = {tabTexture[line[i+2].y].x, tabTexture[line[i+2].y].y};
        //Bx - Ax; By - Ay; Bz - Az;
        point3Df v1 = {B.x - A.x, B.y - A.y, B.z - A.z};
        //Cx - Ax; Cy - Ay; Cz - Az;
        point3Df v2 = {C.x - A.x, C.y - A.y, C.z - A.z};
        //Produit vectorielle du triangle
        vec3Df crossV = crossProduct(v1, v2);
        //Normalisation des vecteurs
        crossV.normalize();
        light.normalize();
        //Calcul du cosinus
        float cos = (crossV.x * light.x + crossV.y * light.y + crossV.z * light.z) / (crossV.norm * light.norm);
        //Produit scalaire entre norme triangle et vecteur de la lumière
        float lighting = crossV.norm * light.norm * cos;
        //Coordonne pour l'écran
        point3Df wA = {world2screen(A.x), world2screen(A.y), world2screen(A.z)};
        point3Df wB = {world2screen(B.x), world2screen(B.y), world2screen(B.z)};
        point3Df wC = {world2screen(C.x), world2screen(C.y), world2screen(C.z)};
        if (lighting >0) {
            point2Df pts[3] = {colA, colB, colC};
            point3Df coord[3] = {wA, wB, wC};
            outils.drawTriangle(coord, image, texture, pts, zbuffer, lighting);
        }
    }
    delete[] zbuffer;
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
}
