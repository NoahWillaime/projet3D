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

vector<point2Df> Outils::boundingBox(point3Df A, point3Df B, point3Df C) {
    int pointX[] = {A.x, B.x, C.x};
    int pointY[] = {A.y, B.y, C.y};
    vector<point2Df> boundingBox;
    point2Df minBox = {getMin(pointX), getMin(pointY)};
    point2Df maxBox = {getMax(pointX), getMax(pointY)};
    boundingBox.push_back(minBox);
    boundingBox.push_back(maxBox);
    return boundingBox;
}

point3Df Outils::barycentric(point2Df p, point3Df A, point3Df B, point3Df C){
    float div = A.x*B.y + B.x*C.y + C.x*A.y - A.y*B.x - B.y*C.x - C.y*A.x;
    float p1 = (p.x*B.y + B.x*C.y + C.x*p.y - p.y*B.x - B.y*C.x - C.y*p.x) / div;
    float p2 = (A.x*p.y + p.x*C.y + C.x*A.y - A.y*p.x - p.y*C.x - C.y*A.x) / div;
    float p3 = (A.x*B.y + B.x*p.y + p.x*A.y - A.y*B.x - B.y*p.x - p.y*A.x) / div;
    point3Df b = {p1, p2, p3};
    return b;
}

point2Df barycentricColor(point2Df *pts, point3Df barCor){
    point2Df bpoint;

    bpoint.x = pts[0].x * barCor.x + pts[1].x * barCor.y + pts[2].x * barCor.z;
    bpoint.y = pts[0].y * barCor.x + pts[1].y * barCor.y + pts[2].y * barCor.z;
    return bpoint;
}

void Outils::drawTriangle(point3Df *coords, TGAImage &image, TGAImage texture, point2Df *pts, int *zbuffer, float lighting) {
    vector<point2Df> bBox = boundingBox(coords[0], coords[1], coords[2]);
    int z = 0;
    TGAColor color;
    point2Df bpoint;
    for (float x = bBox[0].x; x <= bBox[1].x; x++){
        for (float y = bBox[0].y; y <= bBox[1].y; y++){
            point2Df p = {x, y};
            point3Df b = barycentric(p, coords[0], coords[1], coords[2]);
            if (b.x < 0 || b.y < 0 || b.z < 0){
                continue;
            } else {
                z = coords[0].z * b.x + coords[1].z * b.y + coords[2].z * b.z;
                if (zbuffer[int(x+y*800)] < z) {
                    zbuffer[int(x+y*800)] = z;
                    bpoint = barycentricColor(pts, b);
                    bpoint.x *= texture.get_width();
                    bpoint.y *= texture.get_width();
                    color = texture.get(bpoint.x, bpoint.y);
                    color[0] *= lighting;
                    color[1] *= lighting;
                    color[2] *= lighting;
                    image.set(x, y, color);
                }
            }
        }
    }
}
