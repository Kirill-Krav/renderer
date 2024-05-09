#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "vec.hpp"

class Model{
public:
    Model(const char *filename);
    int nverts();
    int nfaces();
    Vec vert(int i);
    std::vector<int> face(int index);
private:
    std::vector<Vec> verts;
    std::vector<std::vector<int>> faces;
};