#include "tek_noise.hpp"

#include "tek_math.hpp"


void tek_noise_init(TekNoise *noise, u32 width, u32 height)
{
    noise->width = width;
    noise->height = height;
    noise->values = (float *) tek_malloc(sizeof(float) * (width * height));
    noise->num_values = width * height;

    for (u32 i = 0; i < noise->num_values; ++i)
    {
	noise->values[i] = math_get_random_float(0, 1);
    }
}

void tek_noise_destroy(TekNoise *noise)
{
    tek_free(noise->values);
}

static double smooth_noise(TekNoise *noise, double x, double y)
{
    //get fractional part of x and y
    double fractX = x - (int) x;
    double fractY = y - (int) y;

    //wrap around
    int x1 = ((int) x + noise->width) % noise->width;
    int y1 = ((int) y + noise->height) % noise->height;

    //neighbor values
    int x2 = (x1 + noise->width - 1) % noise->width;
    int y2 = (y1 + noise->height - 1) % noise->height;

    //smooth the noise with bilinear interpolation
    double value = 0.0;
    value += fractX * fractY * noise->values[y1 * noise->width + x1];
    value += (1 - fractX) * fractY * noise->values[y1 * noise->width + x2];
    value += fractX * (1 - fractY) * noise->values[y2 * noise->width + x1];
    value += (1 - fractX) * (1 - fractY) * noise->values[y2 * noise->width + x2];

    return value;
}

static double turbulence(TekNoise *noise, double x, double y, double size)
{
    double value = 0.0, initialSize = size;

    while (size >= 1)
    {
	value += smooth_noise(noise, x / size, y / size) * size;
	size /= 2.0;
    }

    double res = (0.5 * value / initialSize);

    return res;
}

const float tek_noise_get_value(TekNoise *noise, u32 x, u32 y)
{
    return turbulence(noise, x, y, 16.0);
}
