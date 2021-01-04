#include "tek_assets.hpp"

bool tek_assets_init(TekAssets* assets)
{
	assets->num_materials = 3;
	assets->materials = (TekAssetMaterial*)tek_malloc(sizeof(TekAssetMaterial) * assets->num_materials);

	tek_material_init(&assets->materials[0].material);
	if (!tek_material_add_diffuse_map(&assets->materials[0].material, "data/textures/grid.tga"))
		return false;
	strcpy(assets->materials[0].name, "grid");

	tek_material_init(&assets->materials[1].material);
	if (!tek_material_add_diffuse_map(&assets->materials[1].material, "data/textures/grid.tga"))
		return false;
	strcpy(assets->materials[1].name, "grid2");

	tek_material_init(&assets->materials[2].material);
	assets->materials[2].material.diffuse_color = tek_color_blue();
	strcpy(assets->materials[2].name, "material3");

	assets->num_meshes = 2;
	assets->meshes = (TekAssetMesh*)tek_malloc(sizeof(TekAssetMesh) * assets->num_meshes);

	if (!tek_mesh_load(&assets->meshes[0].mesh, "data/meshes/female.obj"))
		return false;
	strcpy(assets->meshes[0].name, "data/meshes/female.obj");
	if (!tek_mesh_load(&assets->meshes[1].mesh, "data/meshes/male.obj"))
		return false;
	strcpy(assets->meshes[1].name, "data/meshes/male.obj");

	return true;
}

void tek_assets_destroy(TekAssets* assets)
{
	for (u32 i = 0; i < assets->num_materials; ++i)
	{
		tek_material_destroy(&assets->materials[i].material);
	}
	tek_free(assets->materials);

	for (u32 i = 0; i < assets->num_meshes; ++i)
	{
		tek_mesh_destroy(&assets->meshes[i].mesh);
	}
	tek_free(assets->meshes);
}