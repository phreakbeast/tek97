#ifndef _TEK_MAP_HPP
#define _TEK_MAP_HPP

#include "../drawing/tek_drawing.hpp"
#include "../drawing/tek_mesh.hpp"
#include "../drawing/tek_camera.hpp"

typedef struct
{
    u32 mesh_id;
    u32 tex_id;
}StaticMesh;

typedef struct
{
    StaticMesh* mesh;
}MapObject;

typedef struct
{
    TekTexture* textures;
    u32 num_textures;

    TekMesh* meshes;
    u32 num_meshes;
    
    MapObject* objects;
    u32 num_objects;
}Map;

void map_init(Map* map);
void map_destroy(Map* map);
void map_render(Map* map, TekCamera* cam);

#endif
