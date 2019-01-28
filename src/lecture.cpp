//
// Created by noahd on 14/01/2019.
//

#include "lecture.hpp"

using namespace std;

Lecture::Lecture(){};

std::vector<vec3Df> Lecture::readNormal(char *filename) {
    ifstream fichier(filename, ios::in);
    string line;
    vector<vec3Df> tab;
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
                tab.push_back(vec3Df(x, y, z));
            }
            line_parts.clear();
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
    return tab;
}

std::vector<point3Df> Lecture::readTexture(char *filename) {
    ifstream fichier(filename, ios::in);
    string line;
    vector<point3Df> tab;
    vector<string> line_parts;
    string part;

    if (fichier){
        while (getline(fichier, line)){
            if (line[0] == 'v' && line[1] == 't' && line[2] == ' '){
                istringstream iss(line);
                while(getline(iss, part, ' ')){
                    line_parts.push_back(part);
                }
                float x = strtof((line_parts[2]).c_str(), 0);
                float y = strtof((line_parts[3]).c_str(), 0);
                float z = strtof((line_parts[4]).c_str(), 0);
                point3Df p = {x, y, z};
                tab.push_back(p);
            }
            line_parts.clear();
        }
        fichier.close();
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
    return tab;
}

std::vector<std::vector<float> > Lecture::readfile(char *filename) {
    ifstream fichier(filename, ios::in);
    string line;
    vector<vector<float> > tab;
    vector<float> x;
    vector<float> y;
    vector<float> z;

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
                z.push_back(strtof((line_parts[3]).c_str(), 0));
            }
            line_parts.clear();
        }
        fichier.close();
        tab.push_back(x);
        tab.push_back(y);
        tab.push_back(z);
    } else {
        cerr << "Ouverture du fichier " << filename << "impossible !" << endl;
    }
    return tab;
}

std::vector<point2D> Lecture::readline(char *filename) {
    ifstream fichier(filename, ios::in);
    string line;
    vector<point2D> line_parts;
    string part;
    string parts;
    int p1;
    int p2;
    if (fichier){
        while (getline(fichier, line)){
            if (line[0] == 'f'){
                istringstream iss(line);
                while(getline(iss, part, ' ')){
                    if (part[0] != 'f'){
                        istringstream iss2(part);
                        getline(iss2, parts, '/');
                        p1 = -1+atoi((parts).c_str());
                        getline(iss2, parts, '/');
                        p2 = -1+atoi((parts).c_str());
                        point2D p = {p1, p2};
                        line_parts.push_back(p);
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