#include "tek_map.hpp"

#include <string.h>
#include <iostream>
#include <fstream>

bool tek_map_init(TekMap* map)
{
	map->num_materials = 0;
	map->materials = NULL;

	map->num_meshes = 0;
	map->meshes = NULL;

	map->sky = nullptr;

	map->num_objects = 0;

	map->num_brushes = 0;
	map->brushes = NULL;

	map->num_quads = 0;
	map->quads = NULL;

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
	if (map->num_meshes > 0)
	{
		tek_free(map->meshes);
	}
	if (map->num_objects > 0)
	{
		tek_free(map->objects);
	}	

	if (map->num_brushes > 0)
	{
		tek_free(map->brushes);
	}

	if (map->num_quads > 0)
	{
		tek_free(map->quads);
	}
}

void tek_map_render(TekMap* map, TekMeshbuffer* buffer)
{
	//render sky
	if(map->sky)
	{

	}

	//render geometry
	for(u32 i=0; i<map->num_brushes; ++i)
	{
		tek_brush_render(&map->brushes[i],buffer,map);
	}

	for(u32 i=0; i<map->num_quads; ++i)
	{
		tek_quad_render(&map->quads[i],buffer,map);
	}

	for (u32 i = 0; i < map->num_objects; ++i)
	{
		TekMapObject* obj = &map->objects[i];
		if (obj->type == TEK_MAP_OBJECT_STATIC)
		{
			if (obj->static_obj->mesh_id < 0 || obj->static_obj->mat_id < 0)
				continue;

			TekMesh* mesh = &map->meshes[obj->static_obj->mesh_id].mesh;
			TekMaterial* mat = &map->materials[obj->static_obj->mat_id].material;
			tek_mb_draw_mesh(buffer,mesh, mat, &obj->transform);
		}
		if (obj->type == TEK_MAP_OBJECT_BILLBOARD)
		{
			if (obj->bb_obj->mat_id < 0)
				continue;

			TekBillboard* bb = &obj->bb_obj->bb;
			TekMaterial* mat = &map->materials[obj->bb_obj->mat_id].material;
			tek_mb_draw_billboard(buffer, bb, mat, &obj->transform,false);
		}
	}
}

void tek_object_render_shape(TekMapObject* obj, TekShapebuffer* buffer, bool selected)
{
	float offset = 0.5f;
	Vec3 p_original = obj->transform.position;
	Vec3 p0 = vec3_create(p_original.x - offset, p_original.y + offset, p_original.z - offset);
	Vec3 p1 = vec3_create(p_original.x - offset, p_original.y + offset, p_original.z + offset);
	Vec3 p2 = vec3_create(p_original.x + offset, p_original.y + offset, p_original.z + offset);
	Vec3 p3 = vec3_create(p_original.x + offset, p_original.y + offset, p_original.z - offset);

	Vec3 p4 = vec3_create(p_original.x - offset, p_original.y - offset, p_original.z - offset);
	Vec3 p5 = vec3_create(p_original.x - offset, p_original.y - offset, p_original.z + offset);
	Vec3 p6 = vec3_create(p_original.x + offset, p_original.y - offset, p_original.z + offset);
	Vec3 p7 = vec3_create(p_original.x + offset, p_original.y - offset, p_original.z - offset);

	TekColor color = tek_color_white();
	if (selected)
		color = tek_color_red();
	
	tek_shapebuffer_draw_line(buffer, p0, p1, color);
	tek_shapebuffer_draw_line(buffer, p1, p2, color);
	tek_shapebuffer_draw_line(buffer, p2, p3, color);
	tek_shapebuffer_draw_line(buffer, p3, p0, color);
	tek_shapebuffer_draw_line(buffer, p4, p5, color);
	tek_shapebuffer_draw_line(buffer, p5, p6, color);
	tek_shapebuffer_draw_line(buffer, p6, p7, color);
	tek_shapebuffer_draw_line(buffer, p7, p4, color);
	tek_shapebuffer_draw_line(buffer, p0, p4, color);
	tek_shapebuffer_draw_line(buffer, p1, p5, color);
	tek_shapebuffer_draw_line(buffer, p2, p6, color);
	tek_shapebuffer_draw_line(buffer, p3, p7, color);
}

void tek_map_update(TekMap* map, float delta)
{

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
	hdr.num_brushes = map->num_brushes;
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
	for(u32 i=0; i<map->num_brushes; ++i)
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
	map->materials = (TekAssetMaterial*)tek_malloc(sizeof(TekAssetMaterial) * map->num_materials);
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

void tek_map_add_material(TekMap* map, TekAssetMaterial mat)
{
	if (map->num_materials == 0)
	{
		map->num_materials = 1;
		map->materials = (TekAssetMaterial*)tek_malloc(sizeof(TekAssetMaterial));
		map->materials[0] = mat;
	}
	else
	{
		map->num_materials++;
		map->materials = (TekAssetMaterial*)tek_realloc(map->materials, sizeof(TekAssetMaterial) * map->num_materials);
		map->materials[map->num_materials - 1] = mat;
	}
}

void tek_map_add_mesh(TekMap* map, TekAssetMesh mesh)
{
	if (map->num_meshes== 0)
	{
		map->num_meshes = 1;
		map->meshes = (TekAssetMesh*)tek_malloc(sizeof(TekAssetMesh));
		map->meshes[0] = mesh;
		map->meshes[0].mesh.vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * mesh.mesh.num_vertices);
		memcpy(map->meshes[0].mesh.vertices, mesh.mesh.vertices, sizeof(TekMeshVertexData) * mesh.mesh.num_vertices);
		map->meshes[0].mesh.indices= (u32*)tek_malloc(sizeof(u32) * mesh.mesh.num_indices);
		memcpy(map->meshes[0].mesh.indices, mesh.mesh.indices, sizeof(u32) * mesh.mesh.num_indices);
	}
	else
	{
		map->num_meshes++;
		map->meshes= (TekAssetMesh*)tek_realloc(map->meshes, sizeof(TekAssetMesh) * map->num_meshes);
		map->meshes[map->num_meshes - 1] = mesh;
		map->meshes[map->num_meshes - 1].mesh.vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * mesh.mesh.num_vertices);
		memcpy(map->meshes[map->num_meshes - 1].mesh.vertices, mesh.mesh.vertices, sizeof(TekMeshVertexData) * mesh.mesh.num_vertices);
		map->meshes[map->num_meshes - 1].mesh.indices = (u32*)tek_malloc(sizeof(u32) * mesh.mesh.num_indices);
		memcpy(map->meshes[map->num_meshes - 1].mesh.indices, mesh.mesh.indices, sizeof(u32) * mesh.mesh.num_indices);
	}
}

TekBrush* tek_map_add_brush(TekMap* map)
{
	if (map->num_brushes == 0)
	{
		map->num_brushes = 1;
		map->brushes = (TekBrush*)tek_malloc(sizeof(TekBrush));
		tek_brush_init(&map->brushes[0]);
		return &map->brushes[0];
	}
	else
	{
		map->num_brushes++;
		map->brushes = (TekBrush*)tek_realloc(map->brushes, sizeof(TekBrush) * map->num_brushes);
		tek_brush_init(&map->brushes[map->num_brushes - 1]);
		return &map->brushes[map->num_brushes - 1];
	}
}

TekQuad* tek_map_add_quad(TekMap* map)
{
	if (map->num_quads == 0)
	{
		map->num_quads = 1;
		map->quads = (TekQuad*)tek_malloc(sizeof(TekQuad));
		tek_quad_init(&map->quads[0]);
		return &map->quads[0];
	}
	else
	{
		map->num_quads++;
		map->quads = (TekQuad*)tek_realloc(map->quads, sizeof(TekQuad) * map->num_quads);
		tek_quad_init(&map->quads[map->num_quads - 1]);
		return &map->quads[map->num_quads - 1];
	}
}

static int check_materials(TekMap* map, TekAssetMaterial* mat)
{
	bool found = false;
	int id = -1;
	for (u32 i = 0; i < map->num_materials; ++i)
	{
		if (strcmp(map->materials[i].name, mat->name) == 0)
		{
			found = true;
			id = i;
			break;
		}
	}
	if (!found)
	{
		tek_map_add_material(map, *mat);
		id = map->num_materials - 1;
	}
	return id;
}

static int check_meshes(TekMap* map, TekAssetMesh* mesh)
{
	bool found = false;
	int id = -1;
	for (u32 i = 0; i < map->num_meshes; ++i)
	{
		if (strcmp(map->meshes[i].name, mesh->name) == 0)
		{
			found = true;
			id = i;
			break;
		}
	}
	if (!found)
	{
		tek_map_add_mesh(map, *mesh);
		id = map->num_meshes- 1;
	}
	return id;
}

void tek_map_brush_set_material(TekMap* map, TekBrush* brush, TekAssetMaterial* mat, int plane)
{
	int id = check_materials(map, mat);
	brush->planes[plane].mat_id = id;
}

void tek_map_quad_set_material(TekMap* map, TekQuad* quad, TekAssetMaterial* mat)
{
	int id = check_materials(map, mat);
	quad->plane.mat_id = id;
}

void tek_map_object_set_material(TekMap* map, TekMapObject* obj, TekAssetMaterial* mat)
{
	if (obj->type == TEK_MAP_OBJECT_STATIC)
	{
		int id = check_materials(map, mat);
		obj->static_obj->mat_id = id;
	}
	else if (obj->type == TEK_MAP_OBJECT_BILLBOARD)
	{
		int id = check_materials(map, mat);
		obj->bb_obj->mat_id = id;
	}
}

void tek_map_object_set_mesh(TekMap* map, TekMapObject* obj, TekAssetMesh* mesh)
{
	if (obj->type == TEK_MAP_OBJECT_STATIC)
	{
		int id = check_meshes(map, mesh);
		obj->static_obj->mesh_id = id;
	}
}

static void object_init(TekMapObject* obj)
{	
	obj->static_obj = NULL;
	obj->bb_obj = NULL;	
	obj->transform = tek_transform_create(vec3_create(0, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));
}

static TekMapObject* create_new_object(TekMap* map)
{
	TekMapObject* obj = NULL;
	if (map->num_objects == 0)
	{
		map->num_objects = 1;
		map->objects = (TekMapObject*)tek_malloc(sizeof(TekMapObject) * map->num_objects);
		obj = &map->objects[0];
	}
	else
	{
		map->num_objects++;
		map->objects = (TekMapObject*)tek_realloc(map->objects, sizeof(TekMapObject) * map->num_objects);
		obj = &map->objects[map->num_objects - 1];
	}
	object_init(obj);
	return obj;
}

TekMapObject* tek_map_add_static_object(TekMap* map)
{
	TekMapObject* obj = create_new_object(map);
	obj->type = TEK_MAP_OBJECT_STATIC;
	obj->static_obj = (TekStaticObj*)tek_malloc(sizeof(TekStaticObj));
	obj->static_obj->mat_id = -1;
	obj->static_obj->mesh_id = -1;
	return obj;
}

TekMapObject* tek_map_add_bb_object(TekMap* map)
{
	TekMapObject* obj = create_new_object(map);
	obj->type = TEK_MAP_OBJECT_BILLBOARD;
	obj->bb_obj = (TekBillboardObj*)tek_malloc(sizeof(TekBillboardObj));
	obj->bb_obj->mat_id = -1;
	tek_bb_init(&obj->bb_obj->bb,1,1,tek_rect_create(0,0,1,1));
	return obj;
}