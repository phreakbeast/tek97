#include "tek_map.hpp"

#include "../drawing/tek_geometry.hpp"

void map_init(Map* map)
{
    map->num_textures = 1;
    map->textures = (TekTexture*)tek_malloc(sizeof(TekTexture) * map->num_textures);

    tek_tex_load(&map->textures[0], "data/textures/grid.tga", TEK_TEX_FILTER_NONE);

    map->num_meshes = 1;
    map->meshes = (TekMesh*)tek_malloc(sizeof(TekMesh) & map->num_meshes);

    tek_geometry_mesh_plane(&map->meshes[0], 1, 1);

    map->num_objects = 1;
    map->objects = (MapObject*)tek_malloc(sizeof(MapObject) * map->num_objects);
    map->objects[0].mesh = (StaticMesh*)tek_malloc(sizeof(StaticMesh));
    map->objects[0].mesh->mesh_id =0;
    map->objects[0].mesh->tex_id =0;
}

void map_destroy(Map* map)
{
    for(u32 i=0; i<map->num_textures; ++i)
    {
	tek_tex_destroy(&map->textures[i]);
    }
    tek_free(map->textures);

    for(u32 i=0; i<map->num_meshes; ++i)
    {
	tek_mesh_destroy(&map->meshes[i]);
    }
    tek_free(map->meshes);

    for(u32 i=0; i<map->num_objects; ++i)
    {
	
    }
    tek_free(map->objects);
}

void map_render(Map* map, TekCamera* cam)
{
    for(u32 i=0; i<map->num_objects; ++i)
    {
	TekMesh* mesh = &map->meshes[map->objects[i].mesh->mesh_id];
	TekTexture* tex = &map->textures[map->objects[i].mesh->tex_id];
	renderer_render_mesh(mesh, tex->id, NULL, cam);
    }
}
