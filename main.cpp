#include "tgaimage.h"
#include "model.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const int width = 2000;
const int height = 2000;

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

int main()
{
    Model *model = new Model("obj/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec v0 = model->vert(face[j]);
            Vec v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x() + 1.0) * width / 2.0f;
            int y0 = (v0.y() + 1.0) * height / 2.0f;
            int x1 = (v1.x() + 1.0) * width / 2.0f;
            int y1 = (v1.y() + 1.0) * height / 2.0f;
            drawLine(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}