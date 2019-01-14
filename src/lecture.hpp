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

class Lecture {
public:
    Lecture();
    std::vector<int> readline(char *filename);
    std::vector<std::vector<float> > readfile(char *filename);
};


#endif //PROJET3D_LECTURE_HPP
