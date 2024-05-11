#include "tgaimage.h"
#include "model.hpp"

#include <limits>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int width = 10000;
const int height = 10000;
const int depth = 10000;

Model *model = new Model("obj/african_head.obj");


void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color)
{
    bool sw = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        sw = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = std::abs(dy) * 2;
    float error = 0;
    int y = y0;
    int inc = y1 > y0 ? 1 : -1;
    for (int x = x0; x < x1; x++)
    {
        if (sw)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx)
        {
            y += inc;
            error -= 2 * dx;
        }
    }
}

void drawTriangle(Vec<int> &v0, Vec<int> &v1, Vec<int> &v2, Vec<int> &uv0, Vec<int> &uv1, Vec<int> &uv2, TGAImage &image, float lightIntensity, int *zbuffer)
{
    if (v0.y() == v1.y() && v1.y() == v2.y())
    {
        return;
    }
    if (v0.y() > v1.y())
    {
        std::swap(v0, v1);
        std::swap(uv0, uv1);
    }
    if (v1.y() > v2.y())
    {
        std::swap(v1, v2);
        std::swap(uv1, uv2);

    }
    if (v0.y() > v1.y())
    {
        std::swap(v0, v1);
        std::swap(uv0, uv1);
    }
    for (int y = v0.y(); y < v2.y(); y++)
    {
        bool ch = y < v1.y();
        float t1 = ch ? (v1.y() == v0.y() ? 1.0f : static_cast<float>(y - v0.y()) / (v1.y() - v0.y())) : (v2.y() == v1.y() ? 1.0f : static_cast<float>(y - v1.y()) / (v2.y() - v1.y()));
        int x1 = ch ? v0.x() + t1 * (v1.x() - v0.x()) : v1.x() + t1 * (v2.x() - v1.x());
        float t2 = static_cast<float>(y - v0.y()) / (v2.y() - v0.y());
        int x2 = v0.x() + t2 * (v2.x() - v0.x());
        Vec<int> A = v0 + ((v2 - v0) * t2);
        Vec<int> B = ch ? v0 + (v1 - v0) * t1 : v1 + (v2 - v1) * t1;
        Vec<int> uvA = uv0 + (uv2 - uv0) * t2;
        Vec<int> uvB = ch ? uv0 + (uv1 - uv0) * t1 : uv1 + (uv2 - uv1) * t1;
        if (A.x() > B.x())
        {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }
        for (int x = A.x(); x < B.x(); x++)
        {
            float t = A.x() == B.x() ? 1.0 : static_cast<float>(x - A.x()) / (B.x() - A.x());
            if (zbuffer[y * width + x] < A.z() + t * (B.z() - A.z()))
            {
                zbuffer[y * width + x] = A.z() + t * (B.z() - A.z());
                image.set(x, y, model->diffuse(uvA + (uvB - uvA) * t));
            }
        }
    }
}

int main()
{
    int *zbuffer = new int[width * height];

    TGAImage image(width, height, TGAImage::RGB);

    Vec<float> lightDir(0, 0, -1);
    for (int i = 0; i < width * height; i++)
    {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec<int> coordsTriangle[3];
        Vec<float> N;
        float lightIntensity = 0.0;
        for (int j = 0; j < 3; j++)
        {
            coordsTriangle[j] = Vec<int>((model->vert(face[j]).x() + 1.0) * width / 2.0f, (model->vert(face[j]).y() + 1.0) * height / 2.0f, (model->vert(face[j]).z() + 1.0) * depth / 2.0f);
        }
        N = Vec<float>::cross(model->vert(face[2]) - model->vert(face[0]), model->vert(face[1]) - model->vert(face[0])).normalize();
        lightIntensity = Vec<float>::dot(lightDir, N);
        if (lightIntensity > 0.0)
        {
            Vec<int> uv[3];
            for(int k = 0; k < 3; k++){
                uv[k] = model->getuv(i, k);
            }
            drawTriangle(coordsTriangle[0], coordsTriangle[1], coordsTriangle[2], uv[0], uv[1], uv[2], image, lightIntensity, zbuffer);
        }
    }
    
    image.flip_vertically();
    image.write_tga_file("output.tga");

    delete[] zbuffer;
    delete model;
    return 0;
}