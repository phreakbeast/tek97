#include "tek_noise.hpp"

#include "tek_math.hpp"

TekNoise::TekNoise(u32 width, u32 height)
{
	this->width = width;
	this->height = height;
	this->values = (float*)tek_malloc(sizeof(float) * (width * height));
	this->num_values = width * height;

	for (u32 i = 0; i < num_values; ++i)
	{
		values[i] = math_get_random_float(0, 1);
	}
}

TekNoise::~TekNoise()
{
	tek_free(values);
}

double TekNoise::smooth_noise(double x, double y)
{
	//get fractional part of x and y
	double fractX = x - (int) x;
	double fractY = y - (int) y;

	//wrap around
	int x1 = ((int) x + width) % width;
	int y1 = ((int) y + height) % height;

	//neighbor values
	int x2 = (x1 + width - 1) % width;
	int y2 = (y1 + height - 1) % height;

	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX * fractY * values[y1 * width + x1];
	value += (1 - fractX) * fractY * values[y1 * width + x2];
	value += fractX * (1 - fractY) * values[y2 * width + x1];
	value += (1 - fractX) * (1 - fractY) * values[y2 * width + x2];

	return value;
}

double TekNoise::turbulence(double x, double y, double size)
{
	double value = 0.0, initialSize = size;

	while (size >= 1)
	{
		value += smooth_noise(x / size, y / size) * size;
		size /= 2.0;
	}

	double res = (0.5 * value / initialSize);

	return res;
}

const float TekNoise::get_value(u32 x, u32 y)
{
	return turbulence(x, y, 16.0);
}
