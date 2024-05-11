#include "model.hpp"

Model::Model(const char *filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec<float> v;
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            verts.push_back(v);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash >> trash;
            Vec<float> n;
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            norms.push_back(n);
        }
        else if (!line.compare(0, 3, "vt "))
        {
            iss >> trash >> trash;
            Vec<float> newUv;
            for (int i = 0; i < 2; i++)
                iss >> newUv[i];
            uv.push_back(newUv);
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<Vec<int>> f;
            Vec<int> tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                    tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
    std::cerr << "# v# " << verts.size() << " f# " << faces.size() << " vt# " << uv.size() << " vn# " << norms.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap);
}

int Model::nverts()
{
    return verts.size();
}

int Model::nfaces()
{
    return faces.size();
}

Vec<float> Model::vert(int i)
{
    return verts[i];
}

std::vector<int> Model::face(int idx)
{
    std::vector<int> face;
    for (int i = 0; i < faces[i].size(); i++)
    {
        face.push_back(faces[idx][i][0]);
    }
    return face;
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img)
{
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos)
    {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec<int> uv)
{
    return diffusemap.get(uv.x(), uv.y());
}

Vec<int> Model::getuv(int iface, int nvert)
{
    int idx = faces[iface][nvert][1];
    return Vec<int>(uv[idx].x() * diffusemap.get_width(), uv[idx].y() * diffusemap.get_height(), 0);
}