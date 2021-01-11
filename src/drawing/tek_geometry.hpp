#ifndef _TEK_GEOMETRY_HPP
#define _TEK_GEOMETRY_HPP

#include "tek_mesh.hpp"

void tek_geometry_mesh_plane(TekMesh* mesh, float width, float height);
void tek_geometry_mesh_create_box(TekMesh* mesh, float width, float height, float depth);
bool tek_geometry_mesh_load(TekMesh* mesh, const char* filename);

#endif
