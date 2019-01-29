//
// Created by noahd on 14/01/2019.
//

#ifndef PROJET3D_LECTURE_HPP
#define PROJET3D_LECTURE_HPP

#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "tgaimage.h"
#include "outils.hpp"
#include "vec3Df.hpp"


class Lecture {
public:
    Lecture();
    std::vector<point2D> readline(char *filename);
    std::vector<vec3Df> readfile(char *filename);
    std::vector<point3Df> readTexture(char *filename);
    std::vector<vec3Df> readNormal(char *filename);
};


#endif //PROJET3D_LECTURE_HPP
