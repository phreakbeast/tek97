#ifndef _TEK_MATERIAL_HPP
#define _TEK_MATERIAL_HPP

#include "tek_color.hpp"
#include "tek_texture.hpp"

typedef struct
{
	TekColor ambient_color;
	TekColor diffuse_color;
	TekColor specular_color;

	bool has_diffuse_map;
	bool has_specular_map;

	TekTexture diffuse_map;
	TekTexture specular_map;
} TekMaterial;

void tek_material_init(TekMaterial *mat);

void tek_material_destroy(TekMaterial *mat);

void tek_material_add_diffuse_map(TekMaterial* material, const char* tex_name);
void tek_material_add_specular_map(TekMaterial* material, const char* tex_name);

#endif
