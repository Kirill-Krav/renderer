#include "model.hpp"

Model::Model(const char *filename)
{
    std::ifstream fin;
    fin.open(filename, std::ifstream::in);
    if(fin.fail()) return;
    std::string line;
    while(!fin.eof()){
        std::getline(fin, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v ")){
            iss >> trash;
            Vec v;
            for(int i = 0; i < 3; i++){
                iss >> v[i];
            }
            verts.push_back(v);
        }
        else if(!line.compare(0, 2, "f ")){
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while(iss >> idx >> trash >> itrash >> trash >> itrash){
                idx--;
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
}

int Model::nverts(){
    return verts.size();
}

int Model::nfaces(){
    return faces.size();
}

Vec Model::vert(int i){
    return verts[i];
}

std::vector<int> Model::face(int i){
    return faces[i];
}