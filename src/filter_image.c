#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

// Function to normalize the Image such that sum is 1.
void l1_normalize(image im)
{
    float sum = 0;
    for (int i = 0; i < im.w * im.h * im.c; i++)
        sum = sum + im.data[i];

    for (int i = 0; i < im.w * im.h * im.c; i++)
        if (sum != 0)
            im.data[i] = im.data[i] / sum;
        else
            im.data[i] = 1.0 / im.w * im.h;
}

// Function to make a box filter.
image make_box_filter(int w)
{
    image filter = make_image(w, w, 1);
    for (int i = 0; i < w * w; i++)
        filter.data[i] = 1.0;

    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // Asserts that that filter has either 1 channel or channel's equlal to the image
    assert(filter.c == 1 || filter.c == im.c);

    // Make a new empty image.
    image new = make_image(im.w, im.h, im.c);

    for (int c = 0; c < new.c; c++)
    {
        for (int y = 0; y < new.h; y++)
        {
            for (int x = 0; x < new.w; x++)
            {
                // Sets the value of channel for the filter depending on the number of channels in kernel and image.
                int channel_filter;
                if (filter.c == 1)
                    channel_filter = 0;
                else
                    channel_filter = c;

                float val = 0.0;
                int x_temp = x - filter.w / 2;
                int y_temp = y - filter.h / 2;

                // Multiplication with the convolution matrix.
                for (int y_filter = 0; y_filter < filter.h; y_filter++)
                {
                    for (int x_filter = 0; x_filter < filter.w; x_filter++)
                    {
                        val = val + get_pixel(filter, x_filter, y_filter, channel_filter) * get_pixel(im, x_temp + x_filter, y_temp + y_filter, c);
                    }
                }

                // Updating the values in the newer image after convolving.
                new.data[c * new.w *new.h + y *new.w + x] = val;
            }
        }
    }

    // If there is no need to preserve all the channels, then,
    if (!preserve)
    {
        image answer = make_image(im.w, im.h, 1);
        for (int y = 0; y < answer.h; y++)
        {
            for (int x = 0; x < answer.w; x++)
            {
                int temp = y * im.w + x;
                float val = 0.0;
                for (int c = 0; c < new.c; c++)
                {
                    val += new.data[c * new.w *new.h + temp];
                }
                answer.data[temp] = val;
            }
        }
        return answer;
    }
    else
    {
        return new;
    }
}

image make_highpass_filter()
{
    image filter = make_box_filter(3);

    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_box_filter(3);

    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_emboss_filter()
{
    image filter = make_box_filter(3);

    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;

    return filter;
}

image make_gaussian_filter(float sigma)
{
    int width = (int)roundf(sigma * 6);

    // Size should be the nearest odd integer to 6*sigma.
    if (width % 2 == 0)
        width++;

    image filter = make_box_filter(width);
    int x_center = (int)width / 2;
    int y_center = x_center;

    for (int y = 0; y < width; y++)
        for (int x = 0; x < width; x++)
        {
            float a = 1 / (sigma * sigma * TWOPI);
            float b = -((x - x_center) * (x - x_center) + (y - y_center) * (y - y_center)) / (sigma * sigma * 2);
            filter.data[x + y * width] = a * exp(b);
        }

    l1_normalize(filter);
    return filter;
}

// Simple add and subtract functions which proceed only after asserting that both the images have the same dimentions.
image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image ans = make_image(a.w, a.h, a.c);
    for (int k = 0; k < a.w * a.h * a.c; k++)
        ans.data[k] = a.data[k] + b.data[k];

    return ans;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image ans = make_image(a.w, a.h, a.c);
    for (int k = 0; k < a.w * a.h * a.c; k++)
        ans.data[k] = a.data[k] - b.data[k];

    return ans;
}

image make_gx_filter()
{
    image filter = make_box_filter(3);

    filter.data[0] = -1;
    filter.data[1] = 0;
    filter.data[2] = 1;
    filter.data[3] = -2;
    filter.data[4] = 0;
    filter.data[5] = 2;
    filter.data[6] = -1;
    filter.data[7] = 0;
    filter.data[8] = 1;

    return filter;
}

image make_gy_filter()
{
    image filter = make_box_filter(3);

    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;
    filter.data[3] = 0;
    filter.data[4] = 0;
    filter.data[5] = 0;
    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;

    return filter;
}

// Normalization of the pixels in the feature, by confining all values in range of [0,1].

void feature_normalize(image im)
{
    float min = 0.0;
    float max = 0.0;

    // Finding out the max and the min value of the pixel.
    for (int i = 1; i < im.w * im.h * im.c; i++)
    {
        if (im.data[i] > max)
            max = im.data[i];
        if (im.data[i] < min)
            min = im.data[i];
    }

    float width = max - min;

    // If image width is 0, set all data to be 0 as all are equal anyway.
    // Else, normalize the image.
    if (width == 0)
        for (int i = 0; i < im.w * im.h * im.c; i++)
            im.data[i] = 0.0;
    else
        for (int i = 0; i < im.w * im.h * im.c; i++)
            im.data[i] = (im.data[i] - min) / width;
}

// Function to make a sobel filter.
image *sobel_image(image im)
{
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    // Convolve the image using the gx and gy filters.
    image gx_convulated = convolve_image(im, gx_filter, 0);
    image gy_convulated = convolve_image(im, gy_filter, 0);

    // Array of output images which stores the gradient and the magnitude.
    image *out = (image *)malloc(2 * sizeof(image));

    out[0].w = im.w;
    out[0].h = im.h;
    out[0].c = 1;
    out[0].data = calloc(im.w * im.h, sizeof(float));
    out[1].w = im.w;
    out[1].h = im.h;
    out[1].c = 1;
    out[1].data = calloc(im.w * im.h, sizeof(float));

    for (int i = 0; i < im.w * im.h; i++)
    {
        float m = sqrtf(gx_convulated.data[i] * gx_convulated.data[i] + gy_convulated.data[i] * gy_convulated.data[i]);
        float g = atan2(gy_convulated.data[i], gx_convulated.data[i]);
        out[0].data[i] = m;
        out[1].data[i] = g;
    }
    return out;
}

image colorize_sobel(image im)
{
    image *sobel_uncolorized = sobel_image(im);

    feature_normalize(sobel_uncolorized[0]);
    feature_normalize(sobel_uncolorized[1]);

    image sobel_color = make_image(im.w, im.h, 3);


    memcpy(sobel_color.data, sobel_uncolorized[1].data, sizeof(float) * im.w * im.h);
    memcpy(sobel_color.data + im.w * im.h, sobel_uncolorized[1].data, sizeof(float) * im.w * im.h);
    memcpy(sobel_color.data + 2 * im.w * im.h, sobel_uncolorized[0].data, sizeof(float) * im.w * im.h);

    hsv_to_rgb(sobel_color);

    // return convolve_image(sobel_color, make_gaussian_filter(1), 1);
    return sobel_color;
}
