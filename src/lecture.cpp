//
// Created by noahd on 14/01/2019.
//

#include "lecture.hpp"

using namespace std;

Lecture::Lecture(){};

std::vector<std::vector<float> > Lecture::readfile(char *filename) {
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

std::vector<int> Lecture::readline(char *filename) {
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