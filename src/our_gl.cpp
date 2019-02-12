//
// Created by willaime3u on 30/01/19.
//

#include "our_gl.h"

const int depth = 255;

Matrice m1 = Matrice(1, 4);
Matrice m2 = Matrice(4, 4);
Matrice vp = Matrice(4, 4);
Matrice lookat = Matrice(4, 4);
Matrice projection = Matrice(4, 4);

vec3Df crossProduct(vec3Df v1, vec3Df v2){
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vec3Df(x, y, z);
}

vec3Df substractM(vec3Df a, vec3Df b){
    return vec3Df(a.x-b.x, a.y-b.y, a.z-b.z);
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


void get_viewport(float x, float y, float width, float height){
    vp.reset();
    vp.identity();
    vp.set(3,0,x+width/2.f);
    vp.set(3,1,y+height/2.f);
    vp.set(3,2,depth/2.f);
    vp.set(0, 0, width/2.f);
    vp.set(1, 1, height/2.f);
    vp.set(2, 2, depth/2.f);
}

void get_perspective(vec3Df eye, vec3Df center){
    projection.reset();
    projection.identity();
    if (eye.z != 0)
        projection.set(2, 3, -1/substractM(eye, center).norm);
    else
        projection.set(2, 3, 0);
}

point4Df perspective(point3Df point){
    m1.reset();
    m1.set(0, 0, point.x);
    m1.set(0, 1, point.y);
    m1.set(0, 2, point.z);
    m1.set(0, 3, 1);
    m1 = m1.multiply(projection);
    //m1.reduire();
    point4Df p = {m1.get(0, 0), m1.get(0, 1), m1.get(0, 2), m1.get(0, 3)};
    return p;
}

vec3Df viewport(vec3Df point){
    m1.reset();
    m1.set(0, 0, point.x);
    m1.set(0, 1, point.y);
    m1.set(0, 2, point.z);
    m1.set(0, 3, 1);
    m1 = m1.multiply(vp);
    vec3Df p = vec3Df(m1.get(0, 0), m1.get(0, 1), m1.get(0, 2));
    return p;
}

void setLook(vec3Df eye, vec3Df center, vec3Df up){
    vec3Df z = substractM(eye, center);
    z.normalize();
    vec3Df x = crossProduct(up, z);
    x.normalize();
    vec3Df y = crossProduct(z, x);
    y.normalize();
    lookat.reset();
    lookat.identity();
    for (int i = 0; i < 3; i++){
        lookat.set(i, 0, x[i]);
        lookat.set(i, 1, y[i]);
        lookat.set(i, 2, z[i]);
        lookat.set(3, i, -center[i]);
    }
}

point3Df view(vec3Df p){
    m1.reset();
    m1.set(0, 0, p.x);
    m1.set(0, 1, p.y);
    m1.set(0, 2, p.z);
    m1.set(0, 3, 1);
    Matrice rotation = Matrice(4,4);
    rotation.identity();
    rotation.set(0,0,cos(1));
    rotation.set(0,2,sin(1));
    rotation.set(2,0,-sin(1));
    rotation.set(2,2,cos(1));
    m1 = m1.multiply(rotation);

    m1 = m1.multiply(lookat);

    point3Df p2 = {m1.get(0, 0), m1.get(0, 1), m1.get(0, 2)};
    return p2;
}

float getMin(float *param) {
    int min = std::numeric_limits<int>::max();
    for (int i = 0; i < 3; i++){
        if (min > param[i])
            min = param[i];
    }
    return min;
}

float getMax(float *param) {
    int max = std::numeric_limits<int>::min();
    for (int i = 0; i < 3; i++){
        if (max < param[i])
            max = param[i];
    }
    return max;
}

std::vector<point2Df> boundingBox(vec3Df A, vec3Df B, vec3Df C) {
    float pointX[] = {A.x, B.x, C.x};
    float pointY[] = {A.y, B.y, C.y};
    std::vector<point2Df> boundingBox;
    point2Df minBox = {getMin(pointX), getMin(pointY)};
    point2Df maxBox = {getMax(pointX), getMax(pointY)};
    boundingBox.push_back(minBox);
    boundingBox.push_back(maxBox);
    return boundingBox;
}

point3Df barycentric(point2Df p, vec3Df A, vec3Df B, vec3Df C){
    float div = A.x*B.y + B.x*C.y + C.x*A.y - A.y*B.x - B.y*C.x - C.y*A.x;
    float p1 = (p.x*B.y + B.x*C.y + C.x*p.y - p.y*B.x - B.y*C.x - C.y*p.x) / div;
    float p2 = (A.x*p.y + p.x*C.y + C.x*A.y - A.y*p.x - p.y*C.x - C.y*A.x) / div;
    float p3 = (A.x*B.y + B.x*p.y + p.x*A.y - A.y*B.x - B.y*p.x - p.y*A.x) / div;
    point3Df b = {p1, p2, p3};
    return b;
}

void drawTriangle(vec3Df *coords, TGAImage &image, int *zbuffer, IShader &shader) {
    std::vector<point2Df> bBox = boundingBox(coords[0], coords[1], coords[2]);
    int z = 0;
    TGAColor color;
    for (float x = bBox[0].x; x <= bBox[1].x; x++){
        for (float y = bBox[0].y; y <= bBox[1].y; y++){
            point2Df p = {x, y};
            point3Df b = barycentric(p, coords[0], coords[1], coords[2]);
            if (b.x < 0 || b.y < 0 || b.z < 0){
                continue;
            } else {
                z = coords[0].z * b.x + coords[1].z * b.y + coords[2].z * b.z;
                if (x+y*800 > 0 && zbuffer[int(x+y*800)] < z) {
                    zbuffer[int(x+y*800)] = z;
                    if (shader.fragment(b, color));
                        image.set(x, y, color);
                }
            }
        }
    }
}