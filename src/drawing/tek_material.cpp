#include "tek_material.hpp"

#include <stdio.h>

void tek_material_init(TekMaterial *mat)
{
	mat->ambient_color = tek_color_create(20, 20, 20);
	mat->diffuse_color = tek_color_create(100, 100, 100);
	mat->specular_color = tek_color_create(100, 100, 100);

	mat->has_diffuse_map = false;
	mat->has_specular_map = false;
}

void tek_material_destroy(TekMaterial *mat)
{
	if (mat->has_diffuse_map)
	{
		tek_tex_destroy(&mat->diffuse_map);
	}

	if (mat->has_specular_map)
	{
		tek_tex_destroy(&mat->specular_map);
	}
}

void tek_material_add_diffuse_map(TekMaterial* material, const char* tex_name)
{
	tek_tex_load(&material->diffuse_map, tex_name, TEK_TEX_FILTER_NONE);
	material->has_diffuse_map = true;
}

void tek_material_add_specular_map(TekMaterial* material, const char* tex_name)
{
	tek_tex_load(&material->specular_map, tex_name, TEK_TEX_FILTER_NONE);
	material->has_specular_map = true;
}