#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "tgaimage.h"

using namespace std;
const int size = 500;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

vector<int> readline(char *filename){
  ifstream fichier(filename, ios::in);
  string line;
  vector<float> x;
  vector<float> y;
  vector<int> line_parts;
  string part;
  string parts;
  if (fichier){
    while (getline(fichier, line)){
      if (line[0] == 'f'){
	istringstream iss(line);
	while(getline(iss, part, ' ')){
	  if (part[0] != 'f'){
	    istringstream iss2(part);
	    getline(iss2, parts, '/');
	    line_parts.push_back(-1+atoi((parts).c_str()));
	  }
	}
      }
    }
    fichier.close();
  } else {
    cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
  }
  return line_parts;
}

vector<vector<float> > readfile(char *filename){
  ifstream fichier(filename, ios::in);
  string line;
  vector<vector<float> > tab;
  vector<float> x;
  vector<float> y;

  vector<string> line_parts;
  string part;

  if (fichier){
    while (getline(fichier, line)){
      if (line[0] == 'v' && line[1] == ' '){
	istringstream iss(line);
	while(getline(iss, part, ' ')){
	  line_parts.push_back(part);
	}
	x.push_back(strtof((line_parts[1]).c_str(), 0));
	y.push_back(strtof((line_parts[2]).c_str(), 0));
      }
      line_parts.clear();
    }
    fichier.close();
    tab.push_back(x);
    tab.push_back(y);
  } else {
    cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
  }
  return tab;
}

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}

int world2screen(float x) {
  return (x+1)*size/2;
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj\n";
    return -1;
  }
  vector<vector<float> > tab = readfile(argv[1]);
  vector<int> line = readline(argv[1]);
  float x1, x2, x3;
  float y1, y2, y3;
  TGAImage image(size, size, TGAImage::RGB);
  for (int i = 0; i < line.size(); i+=3){
    for (int j=0; j<3; j++) {
      int Ax = world2screen(tab[0][line[i+j]]);
      int Ay = world2screen(tab[1][line[i+j]]);
      int Bx = world2screen(tab[0][line[i+(j+1)%3]]);
      int By = world2screen(tab[1][line[i+(j+1)%3]]);
      drawLine(Ax, Ay, Bx, By, image, white);
    }
  }
  image.flip_vertically();
  image.write_tga_file("output.tga");
}
