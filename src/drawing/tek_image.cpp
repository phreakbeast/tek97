#include "tek_image.hpp"

#include <stdlib.h>
#include <stdio.h>

void tek_img_destroy(TekImage *img)
{
    tek_free(img->pixels);
    img->pixels = nullptr;
}

void tek_img_set_pixel(TekImage *img, int x, int y, Color color)
{
    img->pixels[(x + y * img->width) * img->internal_format + 0] = color.r;
    img->pixels[(x + y * img->width) * img->internal_format + 1] = color.g;
    img->pixels[(x + y * img->width) * img->internal_format + 2] = color.b;
    if (img->internal_format == 4)
	img->pixels[(x + y * img->width) * img->internal_format + 3] = color.a;
}

const Color tek_img_get_pixel(TekImage *img, int x, int y)
{
    u8 r, g, b;
    u8 a = 255;
    r = img->pixels[(x + y * img->width) * img->internal_format + 0];
    g = img->pixels[(x + y * img->width) * img->internal_format + 1];
    b = img->pixels[(x + y * img->width) * img->internal_format + 2];
    if (img->internal_format == 4)
	a = img->pixels[(x + y * img->width) * img->internal_format + 3];

    return Color(r, g, b, a);
}


bool tek_img_load(TekImage *img, const char *filename)
{

    bool success = _tek_load_tga(img, filename);

    return success;
}

bool tek_img_create_blank(TekImage *img, int width, int height, Color color)
{
    img->width = width;
    img->height = height;
    img->format = TEK_IMG_RGB;
    img->internal_format = 3;
    img->pixels = (u8*)tek_malloc(sizeof(u8) * width * height * img->internal_format);
    for (int y = 0; y < height; ++y)
    {
	for (int x = 0; x < width; ++x)
	{
	    tek_img_set_pixel(img, x, y, color);
	}
    }

    return true;
}

void draw_checker(TekImage *img, Color col, int xmin, int xmax, int ymin, int ymax)
{
    int mid_x = xmin + ((xmax - xmin) / 2);
    int mid_y = ymin + ((ymax - ymin) / 2);

    for (int y = ymin; y < mid_y; y++)
    {
	for (int x = xmin; x < mid_x; x++)
	{
	    tek_img_set_pixel(img, x, y, col);
	}
    }

    for (int y = mid_y; y < ymax; y++)
    {
	for (int x = mid_x; x < xmax; x++)
	{
	    tek_img_set_pixel(img, x, y, col);
	}
    }
}

bool
tek_img_create_checker(TekImage *img, Color color1, Color color2, int x_div, int y_div, int width, int height)
{
    tek_img_create_blank(img, width, height, color2);

    int part_width = width / (x_div);
    int part_height = height / (y_div);

    for (int dy = 0; dy < y_div; dy++)
    {
	for (int dx = 0; dx < x_div; dx++)
	{
	    int xmin = dx * part_width;
	    int xmax = xmin + part_width;
	    int ymin = dy * part_height;
	    int ymax = ymin + part_height;

	    draw_checker(img, color1, xmin, xmax, ymin, ymax);
	}
    }

    return true;
}

bool tek_img_create_from_noise(TekImage *img, TekNoise *noise, Color color1, Color color2)
{
    tek_img_create_blank(img, noise->width, noise->height, color1);

    for (u32 y = 0; y < noise->height; ++y)
    {
	for (u32 x = 0; x < noise->width; ++x)
	{
	    float val = tek_noise_get_value(noise,x, y);
	    Color c = color1.lerp(color2, val);
	    tek_img_set_pixel(img, x, y, c);
	}
    }

    return true;
}

