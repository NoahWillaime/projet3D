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

void Outils::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, TGAImage &image, TGAColor color) {
    vector<vector<int> > line = drawLine(x1, y1, x2, y2, image, color);
    drawLine(x2, y2, x3, y3, image, color);
    drawLine(x3, y3, x1, y1, image, color);
    for (int i = 0; i < line[0].size(); i++){
        drawLine(x3, y3, line[0][i], line[1][i], image, color);
    }
}
