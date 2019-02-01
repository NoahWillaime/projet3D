#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include "tgaimage.h"
#include "vec3Df.hpp"
#include "matrice.h"
#include "Model.h"
#include "our_gl.h"

using namespace std;
const int size = 800;
const int depth = 255;

Model *model = NULL;

vec3Df eye = vec3Df(0, 0, 3);
vec3Df up = vec3Df(0, 1, 0);
vec3Df light = vec3Df(0, 0, 1);
vec3Df center = vec3Df(0, 0, 0);

struct GShader : public IShader{
    Matrice texturePts = Matrice(3, 2); //Col1 = A, Col2 = B, Col3 = C

    virtual vec3Df vertex(int i){
        point2D indexA = model->getLine(i);
        point2D indexB = model->getLine(i+1);
        point2D indexC = model->getLine(i+2);
        texturePts.set(0, 0, model->getTabTexture(indexA.y).x); //A.x
        texturePts.set(0, 1, model->getTabTexture(indexA.y).y); //A.y
        texturePts.set(1, 0, model->getTabTexture(indexB.y).x); //B.x
        texturePts.set(1, 1, model->getTabTexture(indexB.y).y); //B.y
        texturePts.set(2, 0, model->getTabTexture(indexC.y).x); //C.x
        texturePts.set(2, 1, model->getTabTexture(indexC.y).y); //C.y
    }

    virtual bool fragment(point3Df barCor, TGAColor &color){
        point2Df bpoint;
        bpoint.x = texturePts.get(0, 0) * barCor.x + texturePts.get(1, 0) * barCor.y + texturePts.get(2, 0) * barCor.z;
        bpoint.y = texturePts.get(0, 1) * barCor.x + texturePts.get(1, 1) * barCor.y + texturePts.get(2, 1) * barCor.z;
        vec3Df test = model->getNormalTexture(bpoint.x, bpoint.y);
        float intensity = test.scalaire(light);
        color = model->diffuse(bpoint)*intensity;
        return true;
    }
};

void drawFace(){
    TGAImage image(size, size, TGAImage::RGB);
    point2D indexA, indexB, indexC;
    light.normalize();
    setLook(eye, center, up);
    get_viewport(size/8, size/8, size*3/4, size*3/4);
    get_perspective(eye);
    GShader shader;
    int *zbuffer = new int[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            zbuffer[i + j * size] = numeric_limits<int>::min();
        }
    }
    for (int i = 0; i < model->getNbLine(); i+=3){
        //Numero du sommet
        indexA = model->getLine(i);
        indexB = model->getLine(i+1);
        indexC = model->getLine(i+2);
        //Sommets
        vec3Df A = model->getTab(indexA.x);
        vec3Df B = model->getTab(indexB.x);
        vec3Df C = model->getTab(indexC.x);

        shader.vertex(i);
        //Transformations
        point3Df wA = viewport(perspective(view(A)));
        point3Df wB = viewport(perspective(view(B)));
        point3Df wC = viewport(perspective(view(C)));



        point3Df coord[3] = {wA, wB, wC};
        drawTriangle(coord, image, zbuffer, shader);
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete[] zbuffer;
}

int main(int argc, char **argv){
  if (argc < 2){
    cerr << "./projet3D filename.obj" << endl;
    return -1;
  }
    model = new Model(argv[1]);
  drawFace();
}
