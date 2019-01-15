#include <stdlib.h>
#include <iostream>
#include <vector>
#include "tgaimage.h"
#include "outils.hpp"
#include "lecture.hpp"


using namespace std;
const int size = 500;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int world2screen(float x) {
  return (x+1)*size/2;
}

/*!
 * Main du programma
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }
    Lecture lecture;
  vector<vector<float> > tab = lecture.readfile(argv[1]);
  vector<int> line = lecture.readline(argv[1]);
  float x1, x2, x3;
  float y1, y2, y3;
  Outils outils;
  TGAImage image(size, size, TGAImage::RGB);
  // for (int i = 0; i < line.size(); i+=3){
  //   for (int j=0; j<3; j++) {
  //     int Ax = world2screen(tab[0][line[i+j]]);
  //     int Ay = world2screen(tab[1][line[i+j]]);
  //     int Bx = world2screen(tab[0][line[i+(j+1)%3]]);
  //     int By = world2screen(tab[1][line[i+(j+1)%3]]);
  //     drawLine(Ax, Ay, Bx, By, image, white);
  //   }
  // }
  outils.drawTriangle(10, 10, 10, 300, 270, 90, image, red);
  image.flip_vertically();
  image.write_tga_file("output.tga");
}
