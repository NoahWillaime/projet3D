//
// Created by willaime3u on 30/01/19.
//

#include "Model.h"

using namespace std;

Model::Model(char *fn) : filename(fn){
    std::stringstream ss, ss2, ss3;
    ss << "../obj/" << filename << "_diffuse.tga";
    ss2 << "../obj/" << filename << "_nm.tga";
    ss3 << "../obj/" << filename << "_spec.tga";
    std::string s = ss.str();
    cout << s << endl;
    texture.read_tga_file(s.c_str());
    texture.flip_vertically();    readfile();
    s = ss2.str();
    cout << s << endl;
    textureNormal.read_tga_file(s.c_str());
    textureNormal.flip_vertically();
    s = ss3.str();
    cout << s << endl;
    textureSpec.read_tga_file(s.c_str());
    textureSpec.flip_vertically();
    readfile();
    readline();
    readTexture();
    readNormal();
}

float Model::specular(point2Df specCord) {
    return textureSpec.get(specCord.x*textureSpec.get_width(), specCord.y*textureSpec.get_width())[0];
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
    v.x /= 127;
    v.y /= 127;
    v.z /= 127;
    v.x -= 1;
    v.y -= 1;
    v.z -= 1;
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
    ifstream fichier(s, ios::in);    string line;
    vector<string> line_parts;
    string part;
    float x,y,z;
    if (fichier){
        while (getline(fichier, line)){
            if (line[0] == 'v' && line[1] == 'n' && line[2] == ' '){
                istringstream iss(line);
                while(getline(iss, part, ' ')){
                    line_parts.push_back(part);
                }
                x = strtof((line_parts[2]).c_str(), 0);
                y = strtof((line_parts[3]).c_str(), 0);
                z = strtof((line_parts[4]).c_str(), 0);
                this->normalVector.push_back(vec3Df(x, y, z));
            }
            line_parts.clear();
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
    vector<string> line_parts;
    string part;

    if (fichier) {
        while (getline(fichier, line)) {
            if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
                istringstream iss(line);
                while (getline(iss, part, ' ')) {
                    line_parts.push_back(part);
                }
                float x = strtof((line_parts[2]).c_str(), 0);
                float y = strtof((line_parts[3]).c_str(), 0);
                float z = strtof((line_parts[4]).c_str(), 0);
                point3Df p = {x, y, z};
                this->tabTexture.push_back(p);
            }
            line_parts.clear();
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
    vector<string> line_parts;
    string part;

    if (fichier) {
        while (getline(fichier, line)) {
            if (line[0] == 'v' && line[1] == ' ') {
                istringstream iss(line);
                while (getline(iss, part, ' ')) {
                    line_parts.push_back(part);
                }
                x = strtof((line_parts[1]).c_str(), 0);
                y = strtof((line_parts[2]).c_str(), 0);
                z = strtof((line_parts[3]).c_str(), 0);
                tab.push_back(vec3Df(x, y, z));
            }
            line_parts.clear();
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
