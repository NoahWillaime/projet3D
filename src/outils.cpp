//
// Created by noahd on 14/01/2019.
//

#include "outils.hpp"

using namespace std;

Outils::Outils() {}

vector<vector<int> > Outils::drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    vector<int> Vx;
    vector<int> Vy;
    vector<vector<int> > tab;

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
            Vx.push_back(y);
            Vy.push_back(x);
        } else {
            image.set(x, y, color);
            Vx.push_back(x);
            Vy.push_back(y);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
    tab.push_back(Vx);
    tab.push_back(Vy);
    return tab;
}

int Outils::getMin(int *param) {
    int min = numeric_limits<int>::max();
    for (int i = 0; i < 3; i++){
        if (min > param[i])
            min = param[i];
    }
    return min;
}

int Outils::getMax(int *param) {
    int max = numeric_limits<int>::min();
    for (int i = 0; i < 3; i++){
        if (max < param[i])
            max = param[i];
    }
    return max;
}

vector<point2D> Outils::boundingBox(point3D A, point3D B, point3D C) {
    int pointX[] = {A.x, B.x, C.x};
    int pointY[] = {A.y, B.y, C.y};
    vector<point2D> boundingBox;
    point2D minBox = {getMin(pointX), getMin(pointY)};
    point2D maxBox = {getMax(pointX), getMax(pointY)};
    boundingBox.push_back(minBox);
    boundingBox.push_back(maxBox);
    return boundingBox;
}

point3Df Outils::barycentric(point2D p, point3D A, point3D B, point3D C){
    float p1 = (p.x - B.x) * (A.y - B.y) - (A.x - B.x) * (p.y - B.y);
    float p2 = (p.x - C.x) * (B.y - C.y) - (B.x - C.x) * (p.y - C.y);
    float p3 = (p.x - A.x) * (C.y - A.y) - (C.x - A.x) * (p.y - A.y);
    point3Df b = {p1, p2, p3};
    return b;
}

void Outils::drawTriangle(point3D A, point3D B, point3D C, TGAImage &image, TGAColor color, int *zbuffer) {
    vector<point2D> bBox = boundingBox(A, B, C);
    int z = 0;
    for (int x = bBox[0].x; x < bBox[1].x; x++){
        for (int y = bBox[0].y; y < bBox[1].y; y++){
            point2D p = {x, y};
            point3Df b = barycentric(p, A, B, C);
            if (b.x < 0 || b.y < 0 || b.z < 0){
                continue;
            } else {
                z = A.z;
                if (zbuffer[x+y*800] < z) {
                    zbuffer[x+y*800] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}
