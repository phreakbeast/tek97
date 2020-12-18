#ifndef _TEK_MAP_HPP
#define _TEK_MAP_HPP

#include "../drawing/tek_mesh.hpp"
#include "../drawing/tek_material.hpp"
#include "../math/tek_transform.hpp"
#include "../drawing/tek_meshbuffer.hpp"

enum TekMapObjectType
{
	TEK_MAP_OBJECT_STATIC,
	TEK_MAP_OBJECT_BILLBOARD,
};

struct TekStaticObj
{
	int mesh_id;
	int mat_id;
};

struct TekBillboardObj
{
	int bb_id;
	int mat_id;
};

struct TekMapObject
{
	TekMapObjectType type;
	TekStaticObj* static_obj;
	TekBillboardObj* bb_obj;

	TekTransform transform;
};

struct TekMap
{
	TekMesh* meshes;
	u32 num_meshes;

	TekMaterial* materials;
	u32 num_materials;

	TekBillboard* bbs;
	u32 num_bbs;

	TekMapObject* objects;
	u32 num_objects;
};

bool tek_map_load(TekMap* map);
void tek_map_destroy(TekMap* map);
void tek_map_render(TekMap* map, TekMeshbuffer* buffer);
void tek_map_update(TekMap* map,float delta);

#endif
