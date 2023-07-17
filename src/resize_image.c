#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, round(x), round(y), c);;
}

image nn_resize(image im, int w, int h)
{
    image new = make_image(w, h, im.c);

    //Fitting new image into Old Coordintates.
    float a_x = (float)im.w / w;
    float b_x = 0.5*a_x - 0.5;
    float a_y = (float)im.h / h;
    float b_y = 0.5*a_y - 0.5;

    //Nearest Neighbour Resize.
    for (int i = 0; i < im.c; i++)
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                new.data[i * w * h + y * w + x] = nn_interpolate(im, a_x * x + b_x, a_y * y + b_y, i);
            }
    return new;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    //Finding the corners of the grid.
    int x_left = floorf(x);
    int x_right = ceilf(x);
    int y_top = floorf(y);
    int y_bottom = ceilf(y);

    //Finding the Pixel Values of the Corners.
    float v1 = get_pixel(im, x_left, y_top, c);
    float v2 = get_pixel(im, x_right, y_top, c);
    float v3 = get_pixel(im, x_left, y_bottom, c);
    float v4 = get_pixel(im, x_right, y_bottom, c);

    //Finding the weights of the grid.
    float d1 = y_bottom - y;
    float d2 = y - y_top;
    float d3 = x - x_left;
    float d4 = x_right - x;

    //Bilinear Interpolation
    float q = v2*d1*d3 + v4*d2*d3 + v1*d1*d4 + v3*d2*d4;

    return q;
}

image bilinear_resize(image im, int w, int h)
{
    image new = make_image(w, h, im.c);

    //Fitting new image into Old Coordintates.
    float a_x = (float)im.w / w;
    float b_x = 0.5*a_x - 0.5;
    float a_y = (float)im.h / h;
    float b_y = 0.5*a_y - 0.5;

    //Bilinear Resize.
    for (int i = 0; i < im.c; i++)
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                new.data[i * w * h + y * w + x] = bilinear_interpolate(im, a_x * x + b_x, a_y * y + b_y, i);
            }
    return new;
}