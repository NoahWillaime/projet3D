//
// Created by willaime3u on 30/01/19.
//

#include "Model.h"

using namespace std;

Model::Model(const char *fn) : filename(fn), glow(false){
    std::stringstream ss, ss2, ss3, ss4;
    ss << "../obj/" << filename << "_diffuse.tga";
    ss2 << "../obj/" << filename << "_nm_tangent.tga";
    ss3 << "../obj/" << filename << "_spec.tga";
    if (fn[0] == 'd') {
        ss4 << "../obj/" << filename << "_glow.tga";
        glow = true;
        std::string s = ss4.str();
        textureGlow.read_tga_file(s.c_str());
        textureGlow.flip_vertically();
    }
    std::string s = ss.str();
    texture.read_tga_file(s.c_str());
    texture.flip_vertically();
    s = ss2.str();
    textureNormal.read_tga_file(s.c_str());
    textureNormal.flip_vertically();
    s = ss3.str();
    textureSpec.read_tga_file(s.c_str());
    textureSpec.flip_vertically();
    readfile();
    readline();
    readTexture();
    readNormal();
}

float Model::specular(point2Df specCord) {
    return textureSpec.get(specCord.x*textureSpec.get_width(), specCord.y*textureSpec.get_width())[0]/1.f;
}

TGAColor Model::glowText(point2Df textureCord) {
    TGAColor color = TGAColor(0,0,0);
    if (glow)
        color = textureGlow.get(textureCord.x*textureGlow.get_width(), textureCord.y * textureGlow.get_width());
    return color;
}

TGAColor Model::diffuse(point2Df textureCord) {
    return texture.get(textureCord.x*texture.get_width(), textureCord.y*texture.get_width());
}

int Model::getNbLine() {
    return line.size();
}

vec3Df Model::getNormalTexture(float x, float y) {
    vec3Df v;
    TGAColor c;
    c = textureNormal.get(x*textureNormal.get_width(), y*textureNormal.get_width());
    v = vec3Df(c[2], c[1], c[0]);
    v.x = v.x/255.f*2.f - 1.f;
    v.y = v.y/255.f*2.f - 1.f;
    v.z = v.z/255.f*2.f - 1.f;
    return v;
}

vec3Df Model::getNormalVector(int i) { return normalVector[i]; }

point2D Model::getLine(int i) { return line[i]; }

vec3Df Model::getTab(int i) { return tab[i]; }

point3Df Model::getTabTexture(int i) { return tabTexture[i]; }

void Model::readNormal() {
    std::stringstream ss;
    ss << "../obj/" << filename << ".obj";
    std::string s = ss.str();
    ifstream fichier(s, ios::in);
    string line;
    float x,y,z;
    if (fichier){
        while (getline(fichier, line)){
            stringstream test(line);
            string option;
            test >> option >> x >> y >> z;
            if (option == "vn") {
                normalVector.push_back(vec3Df(x, y, z));
            }
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
}

void Model::readTexture() {
    std::stringstream ss;
    ss << "../obj/" << filename << ".obj";
    std::string s = ss.str();
    ifstream fichier(s, ios::in);
    string line;
    float x,y;
    if (fichier) {
        while (getline(fichier, line)) {
            stringstream test(line);
            string option;
            test >> option >> x >> y;
            if (option == "vt") {
                point3Df p = {x, y, 0};
                this->tabTexture.push_back(p);
            }
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
}


void Model::readfile(){
    std::stringstream ss;
    ss << "../obj/" << filename << ".obj";
    std::string s = ss.str();
    ifstream fichier(s, ios::in);
    string line;
    float x, y, z;

    if (fichier) {
        while (getline(fichier, line)) {
            stringstream test(line);
            string option;
            test >> option >> x >> y >> z;
            if (option == "v") {
                tab.push_back(vec3Df(x, y, z));
            }
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
}

void Model::readline() {
    std::stringstream ss;
    ss << "../obj/" << filename << ".obj";
    std::string s = ss.str();
    ifstream fichier(s, ios::in);    string line;
    string part;
    string parts;
    int p1;
    int p2;
    if (fichier) {
        while (getline(fichier, line)) {
            if (line[0] == 'f') {
                istringstream iss(line);
                while (getline(iss, part, ' ')) {
                    if (part[0] != 'f') {
                        istringstream iss2(part);
                        getline(iss2, parts, '/');
                        p1 = -1 + atoi((parts).c_str());
                        getline(iss2, parts, '/');
                        p2 = -1 + atoi((parts).c_str());
                        point2D p = {p1, p2};
                        this->line.push_back(p);
                    }
                }
            }
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
}
