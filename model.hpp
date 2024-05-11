#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "vec.hpp"
#include "tgaimage.h"

class Model{
public:
    Model(const char *filename);
    int nverts();
    int nfaces();
    Vec<float> vert(int i);
    Vec<int> getuv(int iface, int nvert);
    TGAColor diffuse(Vec<int> uv);
    std::vector<int> face(int index);
private:
    std::vector<Vec<float>> verts;
    std::vector<std::vector<Vec<int>>> faces;
    std::vector<Vec<float>> norms;
    std::vector<Vec<float>> uv;
    TGAImage diffusemap;
    void load_texture(std::string filename, const char *suffix, TGAImage &image);
};