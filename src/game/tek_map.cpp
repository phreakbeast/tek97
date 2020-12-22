#include "tek_map.hpp"

#include <string.h>
#include <iostream>
#include <fstream>

bool tek_map_init(TekMap* map)
{
	map->num_materials = 1;
	map->materials = (TekMapMaterial*)tek_malloc(sizeof(TekMapMaterial) * map->num_materials);

	tek_material_init(&map->materials[0].material);
	tek_material_add_diffuse_map(&map->materials[0].material, "data/textures/grid.tga");
	strcpy(map->materials[0].name,"material1");

	map->num_meshes = 2;
	map->meshes = (TekMapMesh*)tek_malloc(sizeof(TekMapMesh) * map->num_meshes);

	tek_mesh_load(&map->meshes[0].mesh,"data/meshes/female.obj");
	strcpy(map->meshes[0].name,"data/meshes/female.obj");
	tek_mesh_load(&map->meshes[1].mesh,"data/meshes/male.obj");
	strcpy(map->meshes[1].name,"data/meshes/male.obj");


	map->bbs.push_back(new TekBillboard(3, 1, TekRect(0, 0, 1, 1)));

	map->sky = nullptr;

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
	map->objects[2].transform = tek_transform_create(vec3_create(-2, 1, -2), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

	map->brushes.push_back(TekBrush());

	map->brushes[0].set_material(&map->materials[0].material,0);
	map->brushes[0].set_material(&map->materials[0].material,1);
	map->brushes[0].set_material(&map->materials[0].material,2);
	map->brushes[0].set_material(&map->materials[0].material,3);
	map->brushes[0].set_material(&map->materials[0].material,4);
	map->brushes[0].set_material(&map->materials[0].material,5);

	map->num_quads = 1;
	map->quads = (TekQuad*)tek_malloc(sizeof(TekQuad) * map->num_quads);
	tek_quad_init(&map->quads[0]);
	tek_quad_set_material(&map->quads[0],&map->materials[0].material);

	return true;
}

void tek_map_destroy(TekMap* map)
{
	for (u32 i = 0; i < map->num_materials; ++i)
	{
		tek_material_destroy(&map->materials[i].material);
	}
	tek_free(map->materials);

	for (u32 i = 0; i < map->num_meshes; ++i)
	{
		tek_mesh_destroy(&map->meshes[i].mesh);
	}
	tek_free(map->meshes);

	for (u32 i = 0; i < map->bbs.size(); ++i)
	{
		delete map->bbs[i];
	}

	tek_free(map->objects);
	tek_free(map->quads);
}

void tek_map_render(TekMap* map, TekMeshbuffer* buffer)
{
	//render sky
	if(map->sky)
	{

	}

	//render geometry
	for(u32 i=0; i<map->brushes.size(); ++i)
	{
		map->brushes[i].render(buffer);
	}

	for(u32 i=0; i<map->num_quads; ++i)
	{
		tek_quad_render(&map->quads[i],buffer);
	}

	for (u32 i = 0; i < map->num_objects; ++i)
	{
		TekMapObject* obj = &map->objects[i];
		if (obj->type == TEK_MAP_OBJECT_STATIC)
		{
			TekMesh* mesh = &map->meshes[obj->static_obj->mesh_id].mesh;
			TekMaterial* mat = &map->materials[obj->static_obj->mat_id].material;
			buffer->draw_mesh(mesh, mat, &obj->transform);
		}
		if (obj->type == TEK_MAP_OBJECT_BILLBOARD)
		{
			TekBillboard* bb = map->bbs[obj->bb_obj->bb_id];
			TekMaterial* mat = &map->materials[obj->bb_obj->mat_id].material;
			buffer->draw_billboard(bb, mat, &obj->transform,false);
		}
	}
}

void tek_map_update(TekMap* map, float delta)
{

}

TekBrush* tek_map_add_brush(TekMap* map)
{
	return NULL;
}

TekQuad* tek_map_add_quad(TekMap* map)
{
	map->num_quads++;
	map->quads = (TekQuad*)tek_realloc(map->quads,sizeof(TekQuad) * map->num_quads);
	tek_quad_init(&map->quads[map->num_quads-1]);
	return &map->quads[map->num_quads-1];
}

struct Header
{
	u32 num_materials;
	u32 num_meshes;
	u32 num_brushes;
	u32 num_objects;
};

bool tek_map_save(TekMap* map, const char* name)
{
	Header hdr;
	hdr.num_materials = map->num_materials;
	hdr.num_meshes = map->num_meshes;
	hdr.num_brushes = map->brushes.size();
	hdr.num_objects = map->num_objects;

	char filename[256];
	sprintf(filename,"data/maps/%s.map",name);
	std::ofstream out_file(filename,std::ios::out | std::ios::binary);
	if(!out_file)
	{
		std::cout << "Cannot open file: " << filename <<std::endl;
		return false;
	}

	//write header
	out_file.write((char*)&hdr,sizeof(Header));

	//materials
	for(u32 i=0; i<map->num_materials; ++i)
	{
		out_file.write((char*)&map->materials[i].name,sizeof(char)*128);

		out_file.write((char*)&map->materials[i].material.ambient_color,sizeof(TekColor));
		out_file.write((char*)&map->materials[i].material.diffuse_color,sizeof(TekColor));
		out_file.write((char*)&map->materials[i].material.specular_color,sizeof(TekColor));

		out_file.write((char*)&map->materials[i].material.has_diffuse_map,sizeof(bool));
		out_file.write((char*)&map->materials[i].material.has_specular_map,sizeof(bool));

		out_file.write((char*)&map->materials[i].material.diffuse_name,sizeof(char) * 128);
		out_file.write((char*)&map->materials[i].material.specular_name,sizeof(char) * 128);
	}

	//meshes
	for(u32 i=0; i<map->num_meshes; ++i)
	{

	}

	//brushes
	for(u32 i=0; i<map->brushes.size(); ++i)
	{

	}

	//objects
	for(u32 i=0; i<map->num_objects; ++i)
	{

	}

	out_file.close();
	std::cout << "Map saved" << std::endl;
	return true;
}

bool tek_map_load(TekMap* map, const char* name)
{
	char filename[256];
	sprintf(filename,"data/maps/%s.map",name);

	std::ifstream in_file(filename, std::ios::in | std::ios::binary);
	if(!in_file) {
		std::cout << "Cannot open file: " << filename << std::endl;
		return false;
	}
	Header hdr;
	in_file.read((char *) &hdr, sizeof(Header));

	map->num_materials = hdr.num_materials;
	map->materials = (TekMapMaterial*)tek_malloc(sizeof(TekMapMaterial) * map->num_materials);
	for(u32 i=0; i<map->num_materials; ++i)
	{
		in_file.read((char*)&map->materials[i].name, sizeof(char)*128);

		in_file.read((char*)&map->materials[i].material.ambient_color,sizeof(TekColor));
		in_file.read((char*)&map->materials[i].material.diffuse_color,sizeof(TekColor));
		in_file.read((char*)&map->materials[i].material.specular_color,sizeof(TekColor));

		in_file.read((char*)&map->materials[i].material.has_diffuse_map,sizeof(bool));
		in_file.read((char*)&map->materials[i].material.has_specular_map,sizeof(bool));

		in_file.read((char*)&map->materials[i].material.diffuse_name,sizeof(char) * 128);
		in_file.read((char*)&map->materials[i].material.specular_name,sizeof(char) * 128);
		if(map->materials[i].material.has_diffuse_map)
		{
			tek_material_add_diffuse_map(&map->materials[i].material,map->materials[i].material.diffuse_name);
		}

		if(map->materials[i].material.has_specular_map)
		{
			tek_material_add_specular_map(&map->materials[i].material,map->materials[i].material.specular_name);
		}
	}

	in_file.close();
	if(!in_file.good()) {
		std::cout << "Error occurred at reading time!" << std::endl;
		return false;
	}
	return true;
}