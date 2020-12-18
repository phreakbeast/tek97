#include "tek_map.hpp"

bool tek_map_load(TekMap* map)
{
	map->num_materials = 1;
	map->materials = (TekMaterial*)tek_malloc(sizeof(TekMaterial) * map->num_materials);

	tek_material_init(&map->materials[0]);
	tek_material_add_diffuse_map(&map->materials[0], "data/textures/grid.tga");

	map->num_meshes = 2;
	map->meshes = (TekMesh*)tek_malloc(sizeof(TekMesh) * map->num_meshes);

	tek_mesh_create_box(&map->meshes[0], 1, 1, 1);
	tek_mesh_create_plane(&map->meshes[1], 10, 10, 5, 5);

	map->num_bbs = 1;
	map->bbs = (TekBillboard*)tek_malloc(sizeof(TekBillboard) * map->num_bbs);

	tek_bb_init(&map->bbs[0], 3, 1, tek_rect_create(0, 0, 1, 1));

	map->num_objects = 3;
	map->objects = (TekMapObject*)tek_malloc(sizeof(TekMapObject) * map->num_objects);

	map->objects[0].type = TEK_MAP_OBJECT_STATIC;
	map->objects[0].static_obj = (TekStaticObj*)tek_malloc(sizeof(TekStaticObj));
	map->objects[0].static_obj->mat_id = 0;
	map->objects[0].static_obj->mesh_id = 0;
	map->objects[0].transform = tek_transform_create(vec3_create(0,0,0),vec3_create(0,0,0),vec3_create(1,1,1));

	map->objects[1].type = TEK_MAP_OBJECT_STATIC;
	map->objects[1].static_obj = (TekStaticObj*)tek_malloc(sizeof(TekStaticObj));
	map->objects[1].static_obj->mat_id = 0;
	map->objects[1].static_obj->mesh_id = 1;
	map->objects[1].transform = tek_transform_create(vec3_create(-5, 0, -5), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

	map->objects[2].type = TEK_MAP_OBJECT_BILLBOARD;
	map->objects[2].bb_obj = (TekBillboardObj*)tek_malloc(sizeof(TekBillboardObj));
	map->objects[2].bb_obj->mat_id = 0;
	map->objects[2].bb_obj->bb_id = 0;
	map->objects[2].transform = tek_transform_create(vec3_create(0, 1, 1), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

	return true;
}

void tek_map_destroy(TekMap* map)
{
	for (u32 i = 0; i < map->num_materials; ++i)
	{
		tek_material_destroy(&map->materials[i]);
	}
	tek_free(map->materials);

	for (u32 i = 0; i < map->num_meshes; ++i)
	{
		tek_mesh_destroy(&map->meshes[i]);
	}
	tek_free(map->meshes);

	for (u32 i = 0; i < map->num_bbs; ++i)
	{
		tek_bb_destroy(&map->bbs[i]);
	}
	tek_free(map->bbs);

	tek_free(map->objects);
}

void tek_map_render(TekMap* map, TekMeshbuffer* buffer)
{
	for (u32 i = 0; i < map->num_objects; ++i)
	{
		TekMapObject* obj = &map->objects[i];
		if (obj->type == TEK_MAP_OBJECT_STATIC)
		{
			TekMesh* mesh = &map->meshes[obj->static_obj->mesh_id];
			TekMaterial* mat = &map->materials[obj->static_obj->mat_id];
			buffer->draw_mesh(mesh, mat, &obj->transform);
		}
		if (obj->type == TEK_MAP_OBJECT_BILLBOARD)
		{
			TekBillboard* bb = &map->bbs[obj->bb_obj->bb_id];
			TekMaterial* mat = &map->materials[obj->bb_obj->mat_id];
			buffer->draw_billboard(bb, mat, &obj->transform,false);
		}
	}
}

void tek_map_update(TekMap* map, float delta)
{

}