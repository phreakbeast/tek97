#ifndef _TEK_ASSETS_HPP
#define _TEK_ASSETS_HPP

#include "../drawing/tek_material.hpp"
#include "../drawing/tek_mesh.hpp"

typedef struct
{
	TekMaterial material;
	char name[128];
}TekAssetMaterial;

typedef struct
{
	TekMesh mesh;
	char name[128];
}TekAssetMesh;

typedef struct
{
	TekAssetMaterial* materials;
	u32 num_materials;

	TekAssetMesh* meshes;
	u32 num_meshes;
}TekAssets;

bool tek_assets_init(TekAssets* assets);
void tek_assets_destroy(TekAssets* assets);

#endif