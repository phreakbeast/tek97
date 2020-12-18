#ifndef _TEK_SHAPE_HPP
#define _TEK_SHAPE_HPP

#include "../math/tek_math.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"
#include "tek_mesh.hpp"

typedef struct
{
    Vec3 position;
    Vec3 color;
} TekShapeVertexData;

typedef struct
{
    u32 vao;
    u32 vbo;
    u32 ibo;
    u32 num_vertices;
    u32 num_indices;
} TekShape;


void tek_shape_destroy(TekShape *shape);

void tek_shape_render(TekShape *shape);

void tek_shape_create(TekShape *shape, TekShapeVertexData *vertices, u32 num_vertices, u32 *indices, u32 num_indices);

void tek_shape_create_line(TekShape *shape, Vec3 p0, Vec3 p1, TekColor color);

void tek_shape_create_box(TekShape *shape, Vec3 min, Vec3 max, TekColor color);

void
tek_shape_create_circle(TekShape *shape, Vec3 center, float radius, TekColor color, u32 segments, float start_angle,
                        float end_angle);

void tek_shape_create_bone(TekShape *shape, float width, float height, TekColor color);

void tek_shape_create_grid(TekShape *shape, float width, float height, u32 segs_x, u32 segs_y, TekColor color);

void tek_shape_create_normals_for_mesh(TekShape *shape, TekMesh *mesh, TekColor color, float length);

#endif