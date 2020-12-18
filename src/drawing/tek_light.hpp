#ifndef _TEK_LIGHT_HPP
#define _TEK_LIGHT_HPP

#include "tek_color.hpp"
#include "../math/tek_math.hpp"

typedef struct
{
	Vec3 direction;
	Vec3 color;
	float intensity;
} TekDirectionalLight;

typedef struct
{
	Vec3 position;
	Vec3 color;
	float range;
}TekPointLight;


struct TekLightManager
{
	TekDirectionalLight dlight;
	bool use_dlight;
	TekPointLight plights[8];
	u32 num_plights;
};

#endif
