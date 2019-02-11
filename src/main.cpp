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
int *shadowbuffer = new int[size * size];

vec3Df eye = vec3Df(1, 1, 3);
vec3Df up = vec3Df(0, 1, 0);
vec3Df light = vec3Df(1,1, 1);
vec3Df center = vec3Df(0, 0, 0);

struct DepthShader : public IShader{
    Matrice varying = Matrice(3, 3);

    virtual vec3Df vertex(int i, int j) {
        point2D indexA = model->getLine(i + j);
        vec3Df A = model->getTab(indexA.x);
        point4Df wA = perspective(view(A));
        vec3Df vpT = viewport(vec3Df(wA.x, wA.y, wA.z));
        varying.set(j, 0, vpT.x);
        varying.set(j, 1, vpT.y);
        varying.set(j, 2, vpT.z);
        return vpT;
    }

    virtual bool fragment(point3Df barCor, TGAColor &color){
        vec3Df p;
        p.x = varying.get(0, 0)  * barCor.x + varying.get(1, 0) * barCor.y + varying.get(2, 0) * barCor.z;
        p.y = varying.get(0, 1)  * barCor.x + varying.get(1, 1) * barCor.y + varying.get(2, 1) * barCor.z;
        p.z = varying.get(0, 2)  * barCor.x + varying.get(1, 2) * barCor.y + varying.get(2, 2) * barCor.z;
        color = TGAColor(255, 255, 255) * (p.z / depth);
        return true;
    }
};

struct GShader : public IShader{
    Matrice texturePts = Matrice(3, 2); //Col1 = A, Col2 = B, Col3 = C
    Matrice MIT = Matrice(4, 4);
    Matrice M = Matrice(4, 4);
    Matrice M_Shadow = Matrice(4, 4);
    Matrice temp = Matrice(1, 4);
    Matrice l = Matrice(1,4);
    Matrice varying = Matrice(3, 3);
    Matrice norm_tri = Matrice(3, 3);
    Matrice varying_norm = Matrice(3, 3);

    virtual vec3Df vertex(int i, int j){
        point2D index = model->getLine(i+j);
        texturePts.set(j, 0, model->getTabTexture(index.y).x); //A.x
        texturePts.set(j, 1, model->getTabTexture(index.y).y); //A.y
        point4Df p = perspective(view(model->getTab(index.x)));
        varying.set(j, 0, p.x);
        varying.set(j, 1, p.y);
        varying.set(j, 2, p.z);
        norm_tri.set(j, 0, p.x/p.w);
        norm_tri.set(j, 1, p.y/p.w);
        norm_tri.set(j, 2, p.z/p.w);
        vec3Df vn = model->getNormalVector(index.x);
        Matrice m = Matrice(1, 4);
        m.set(0,0,vn.x);
        m.set(0,1,vn.y);
        m.set(0,2,vn.z);
        m.set(0,3,0.f);
        m.multiply(MIT);
        varying_norm.set(j, 0, m.get(0,0));
        varying_norm.set(j, 1, m.get(0,1));
        varying_norm.set(j, 2, m.get(0,2));
       varying_norm.print();
    //    MIT.print();
        std::cout << "==" << std::endl;
        if (j == 2){
            Matrice mTest = Matrice(1, 3);
            vec3Df barTest = vec3Df(1,1,1);
            vec3Df barCor= vec3Df(1,1,1);
            mTest.setCol(0, barTest);
            mTest.multiply(varying_norm);
            vec3Df h = vec3Df(mTest.get(0,0), mTest.get(0,1),mTest.get(0,2));
            h.normalize();
          //  std::cout <<h.x << " / " << h.y << " / " <<  h.z<<std::endl;
        }
        return vec3Df(p.x, p.y,p.z);
    }

    virtual bool fragment(point3Df barCor, TGAColor &color){
        Matrice A = Matrice(3, 3);
        A.set(0, 0, norm_tri.get(1, 0) - norm_tri.get(0, 0));
        A.set(1, 0, norm_tri.get(1, 1) - norm_tri.get(0, 1));
        A.set(2, 0, norm_tri.get(1, 2) - norm_tri.get(0, 2));
        A.set(0, 1, norm_tri.get(2, 0) - norm_tri.get(0, 0));
        A.set(1, 1, norm_tri.get(2, 1) - norm_tri.get(0, 1));
        A.set(2, 1, norm_tri.get(2, 2) - norm_tri.get(0, 2));
        vec3Df bn;
        bn.x = varying_norm.get(0,0) * barCor.x + varying_norm.get(1, 0) * barCor.y + varying_norm.get(2, 0) * barCor.z;
        bn.y = varying_norm.get(0,1) * barCor.x + varying_norm.get(1, 1) * barCor.y + varying_norm.get(2, 1) * barCor.z;
        bn.z = varying_norm.get(0,2) * barCor.x + varying_norm.get(1, 2) * barCor.y + varying_norm.get(2, 2) * barCor.z;

        bn.normalize();

        A.setRow(2, bn);
        Matrice AI = A.inverse3();

        vec3Df ti = vec3Df(texturePts.get(1, 0) - texturePts.get(0, 0), texturePts.get(2, 0) - texturePts.get(0, 0), 0.f); //OK
        vec3Df tj = vec3Df(texturePts.get(1, 1) - texturePts.get(0, 1), texturePts.get(2, 1) - texturePts.get(0, 1), 0.f); //OK

        Matrice mi = Matrice(1, 3);
        mi.setCol(0, ti);
        mi.multiply(AI);
        Matrice mj = Matrice(1, 3);
        mj.setCol(0, tj);
        mj.multiply(AI);
        vec3Df i = vec3Df(mi.get(0,0), mi.get(0, 1), mi.get(0, 2));
        vec3Df j = vec3Df(mj.get(0,0), mj.get(0, 1), mj.get(0, 2));
        mi.delMatrice();
        mj.delMatrice();
        Matrice B = Matrice(3, 3);
        i.normalize();
        j.normalize();
        B.setCol(0,i);
        B.setCol(1,j);
        B.setCol(2,bn);
        point2Df bpoint;
        bpoint.x = texturePts.get(0, 0) * barCor.x + texturePts.get(1, 0) * barCor.y + texturePts.get(2, 0) * barCor.z;
        bpoint.y = texturePts.get(0, 1) * barCor.x + texturePts.get(1, 1) * barCor.y + texturePts.get(2, 1) * barCor.z;
        vec3Df test = model->getNormalTexture(bpoint.x, bpoint.y);
        Matrice t = Matrice(1, 3);
        t.setCol(0, test);
        t.multiply(B);
        vec3Df n2 = vec3Df(B.get(0, 0) * test.x + B.get(1, 0) * test.y + B.get(2, 0) * test.z, B.get(0, 1) * test.x + B.get(1, 1) * test.y + B.get(2, 1) * test.z, B.get(0, 2) * test.x + B.get(1, 2) * test.y + B.get(2, 2) * test.z);
        vec3Df n = vec3Df(t.get(0, 0), t.get(0, 1), t.get(0, 2));
        n2.normalize();
        n.normalize();
        /*
        Matrice m = Matrice(1, 4);
        m.set(0, 0, varying.get(0, 0)  * barCor.x + varying.get(1, 0) * barCor.y + varying.get(2, 0) * barCor.z);
        m.set(0, 1, varying.get(0, 1)  * barCor.x + varying.get(1, 1) * barCor.y + varying.get(2, 1) * barCor.z);
        m.set(0, 2, varying.get(0, 2)  * barCor.x + varying.get(1, 2) * barCor.y + varying.get(2, 2) * barCor.z);
        m.set(0, 3, 1);
        m.multiply(M_Shadow);
        vec3Df v(m.get(0, 0) / m.get(0, 3), m.get(0, 1) / m.get(0, 3), m.get(0, 2) / m.get(0, 3));
        int index = int(v[0]) + int(v[1]) * size;
        float shadow = .3 + .7*(shadowbuffer[index] < v[2]+43.34);
        point2Df bpoint;
        bpoint.x = texturePts.get(0, 0) * barCor.x + texturePts.get(1, 0) * barCor.y + texturePts.get(2, 0) * barCor.z;
        bpoint.y = texturePts.get(0, 1) * barCor.x + texturePts.get(1, 1) * barCor.y + texturePts.get(2, 1) * barCor.z;
        vec3Df test = model->getNormalTexture(bpoint.x, bpoint.y);
        temp.set(0, 0, test.x);
        temp.set(0, 1, test.y);
        temp.set(0, 2, test.z);
        temp.set(0, 3, 0);
        l.set(0, 0, light.x);
        l.set(0, 1, light.y);
        l.set(0, 2, light.z);
        l.set(0, 3, 1);
        temp.multiply(MIT);
        l.multiply(M);
        vec3Df v1 = vec3Df(temp.get(0, 0), temp.get(0, 1), temp.get(0, 2));
        vec3Df v2 = vec3Df(l.get(0, 0), l.get(0, 1), l.get(0, 2));
        v1.normalize();
        v2.normalize();
        vec3Df r = v1.mult(v1.scalaire(v2) * 2.f);
        r.soustraction(v2);
        r.normalize();
        float spec = pow(max(r.z, 0.0f), model->specular(bpoint)+100);*/
     //   float diff = max(0.f, v1.scalaire(v2));

        float diff = max(0.f, n2.scalaire(light)); //PROBLEME ICI

        //   cout << n.scalaire(light) <<endl;
        TGAColor c = model->diffuse(bpoint);
        color = c;
        for (int i = 0; i < 3; i++){
        //    color[i] = min<float>(5 + c[i]*(diff+2*spec), 255);
            color[i] = c[i] *diff; //1 POUR TESTER
        }
      //  cout << (float)diff << " : " << (int)c[0] << " / " << (int)c[1] << " / " << (int)c[2] << endl;

        return true;
    }
};

void drawFace(){
    TGAImage image(size, size, TGAImage::RGB);
    TGAImage depthI(size, size, TGAImage::RGB);
   // light.normalize();
    GShader shader;
    DepthShader depthShader;
    int *zbuffer = new int[size * size];
    vec3Df coord[3];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            zbuffer[i + j * size] = numeric_limits<int>::min();
            shadowbuffer[i + j * size] = numeric_limits<int>::min();
        }
    }

    //Shadow
    setLook(light, center, up);
    get_viewport(size/8, size/8, size*3/4, size*3/4);
    get_perspective(vec3Df());
    for (int i = 0; i < model->getNbLine(); i+=3){
        for (int j = 0; j < 3; j++){
            coord[j] = depthShader.vertex(i, j);
        }
        drawTriangle(coord, depthI, shadowbuffer, depthShader);
    }
    depthI.flip_vertically();
    depthI.write_tga_file("depth.tga");

    //Frame
    setLook(eye, center, up);
    get_viewport(size/8, size/8, size*3/4, size*3/4);
    get_perspective(eye);
    shader.M = lookat.multiplyCarre(projection);
    shader.MIT = shader.M.transpose().inverse();
    shader.MIT.set(3, 0, -0);
    shader.MIT.set(3, 1, 0);
    shader.M_Shadow = shader.M.multiplyCarre(vp.multiplyCarre(lookat.multiplyCarre(projection)).inverse());

    Matrice l = Matrice(1, 4);
    l.set(0, 0, light.x);
    l.set(0, 1, light.y);
    l.set(0, 2, light.z);
    l.set(0, 3, 1);
    l.multiply(shader.M);
    light = vec3Df(l.get(0, 0), l.get(0, 1), l.get(0, 2));
    light.normalize();
    for (int i = 0; i < model->getNbLine(); i+=3){
        for (int j = 0; j < 3; j++){
            coord[j] = viewport(shader.vertex(i, j));
        }
        drawTriangle(coord, image, zbuffer, shader);
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete[] zbuffer;
}

int main(int argc, char **argv){
    if (argc < 2){
        cerr << "./projet3D [Model Name]" << endl;
        return -1;
    }
    model = new Model(argv[1]);
    drawFace();


}
