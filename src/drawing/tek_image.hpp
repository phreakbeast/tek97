#ifndef _TEK_IMAGE_HPP
#define _TEK_IMAGE_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_noise.hpp"
#include "tek_color.hpp"
#include <stdio.h>

typedef enum
{
    TEK_IMG_NONE,
    TEK_IMG_RGB,
    TEK_IMG_RGBA
} TekImageFormat;

typedef struct
{
    u32 width;
    u32 height;
    TekImageFormat format;
    int internal_format;
    u8 *pixels;
} TekImage;

void tek_img_destroy(TekImage *img);

void tek_img_set_pixel(TekImage* img, int x, int y, Color color);

const Color tek_img_get_pixel(TekImage* img, int x, int y);

bool tek_img_load(TekImage *img, const char *filename);

bool tek_img_create_blank(TekImage *img, int width, int height, Color color);

bool
tek_img_create_checker(TekImage *img, Color color1, Color color2, int x_div, int y_div, int width, int height);

bool tek_img_create_from_noise(TekImage *img, TekNoise *noise, Color color1, Color color2);

//TODO: handle top and bottom origins
bool _tek_load_tga(TekImage *img, const char *filename);


#endif
