#include "tek_map.hpp"

#include "../drawing/tek_drawing.hpp"

static void update_vertices(TekBrush* brush);

void tek_brush_init(TekBrush* brush)
{
	brush->points[0] = vec3_create(0,1,0);
	brush->points[1] = vec3_create(0,1,1);
	brush->points[2] = vec3_create(1,1,1);
	brush->points[3] = vec3_create(1,1,0);
	brush->points[4] = vec3_create(0,0,0);
	brush->points[5] = vec3_create(0,0,1);
	brush->points[6] = vec3_create(1,0,1);
	brush->points[7] = vec3_create(1,0,0);

	brush->lines[0].p0 = 0;
	brush->lines[0].p1 = 3;
	brush->lines[1].p0 = 0;
	brush->lines[1].p1 = 1;
	brush->lines[2].p0 = 1;
	brush->lines[2].p1 = 2;
	brush->lines[3].p0 = 2;
	brush->lines[3].p1 = 3;
	brush->lines[4].p0 = 4;
	brush->lines[4].p1 = 7;
	brush->lines[5].p0 = 4;
	brush->lines[5].p1 = 5;
	brush->lines[6].p0 = 5;
	brush->lines[6].p1 = 6;
	brush->lines[7].p0 = 6;
	brush->lines[7].p1 = 7;
	brush->lines[8].p0 = 0;
	brush->lines[8].p1 = 4;
	brush->lines[9].p0 = 1;
	brush->lines[9].p1 = 5;
	brush->lines[10].p0 = 2;
	brush->lines[10].p1 = 6;
	brush->lines[11].p0 = 3;
	brush->lines[11].p1 = 7;

	//front plane
	brush->planes[0].p0 = 1;
	brush->planes[0].p1 = 5;
	brush->planes[0].p2 = 6;
	brush->planes[0].p3 = 2;
	brush->planes[0].l0 = 5;
	brush->planes[0].l1 = 9;
	brush->planes[0].l2 = 6;
	brush->planes[0].l3 = 10;

	//left plane
	brush->planes[1].p0 = 0;
	brush->planes[1].p1 = 4;
	brush->planes[1].p2 = 5;
	brush->planes[1].p3 = 1;
	brush->planes[1].l0 = 1;
	brush->planes[1].l1 = 8;
	brush->planes[1].l2 = 5;
	brush->planes[1].l3 = 9;

	//right plane
	brush->planes[2].p0 = 2;
	brush->planes[2].p1 = 6;
	brush->planes[2].p2 = 7;
	brush->planes[2].p3 = 3;
	brush->planes[2].l0 = 3;
	brush->planes[2].l1 = 10;
	brush->planes[2].l2 = 7;
	brush->planes[2].l3 = 11;

	//back plane
	brush->planes[3].p0 = 3;
	brush->planes[3].p1 = 7;
	brush->planes[3].p2 = 4;
	brush->planes[3].p3 = 0;
	brush->planes[3].l0 = 0;
	brush->planes[3].l1 = 11;
	brush->planes[3].l2 = 4;
	brush->planes[3].l3 = 8;

	//top plane
	brush->planes[4].p0 = 0;
	brush->planes[4].p1 = 1;
	brush->planes[4].p2 = 2;
	brush->planes[4].p3 = 3;
	brush->planes[4].l0 = 0;
	brush->planes[4].l1 = 1;
	brush->planes[4].l2 = 2;
	brush->planes[4].l3 = 3;

	//bottom plane
	brush->planes[5].p0 = 6;
	brush->planes[5].p1 = 5;
	brush->planes[5].p2 = 4;
	brush->planes[5].p3 = 7;
	brush->planes[5].l0 = 6;
	brush->planes[5].l1 = 5;
	brush->planes[5].l2 = 4;
	brush->planes[5].l3 = 7;

	brush->planes[0].mat_id = -1;
	brush->planes[1].mat_id = -1;
	brush->planes[2].mat_id = -1;
	brush->planes[3].mat_id = -1;
	brush->planes[4].mat_id = -1;
	brush->planes[5].mat_id = -1;

	brush->planes[0].uv0 = vec2_create(0,1);
	brush->planes[0].uv1 = vec2_create(0,0);
	brush->planes[0].uv2 = vec2_create(1,0);
	brush->planes[0].uv3 = vec2_create(1,1);

	brush->planes[1].uv0 = vec2_create(0,1);
	brush->planes[1].uv1 = vec2_create(0,0);
	brush->planes[1].uv2 = vec2_create(1,0);
	brush->planes[1].uv3 = vec2_create(1,1);

	brush->planes[2].uv0 = vec2_create(0,1);
	brush->planes[2].uv1 = vec2_create(0,0);
	brush->planes[2].uv2 = vec2_create(1,0);
	brush->planes[2].uv3 = vec2_create(1,1);

	brush->planes[3].uv0 = vec2_create(0,1);
	brush->planes[3].uv1 = vec2_create(0,0);
	brush->planes[3].uv2 = vec2_create(1,0);
	brush->planes[3].uv3 = vec2_create(1,1);

	brush->planes[4].uv0 = vec2_create(0,1);
	brush->planes[4].uv1 = vec2_create(0,0);
	brush->planes[4].uv2 = vec2_create(1,0);
	brush->planes[4].uv3 = vec2_create(1,1);

	brush->planes[5].uv0 = vec2_create(0,1);
	brush->planes[5].uv1 = vec2_create(0,0);
	brush->planes[5].uv2 = vec2_create(1,0);
	brush->planes[5].uv3 = vec2_create(1,1);

	update_vertices(brush);
}

void tek_brush_render(TekBrush* brush, TekMeshbuffer* buffer, TekMap* map)
{
	for(u32 i=0; i< 6; ++i)
	{
		if(brush->planes[i].mat_id >= 0)
		{
			TekMaterial* mat = &map->materials[brush->planes[i].mat_id].material;
			tek_mb_draw_vertices(buffer,brush->planes[i].vertices,6, mat);
		}
	}
}

void tek_brush_render_shape(TekBrush* brush, TekShapebuffer* buffer, int mode, int selected)
{
	if(mode == 1)
	{
		TekColor color = tek_color_white();
		tek_shapebuffer_draw_line(buffer, brush->points[0], brush->points[1], color);
		tek_shapebuffer_draw_line(buffer, brush->points[1], brush->points[2], color);
		tek_shapebuffer_draw_line(buffer, brush->points[2], brush->points[3], color);
		tek_shapebuffer_draw_line(buffer, brush->points[3], brush->points[0], color);
		tek_shapebuffer_draw_line(buffer, brush->points[4], brush->points[5], color);
		tek_shapebuffer_draw_line(buffer, brush->points[5], brush->points[6], color);
		tek_shapebuffer_draw_line(buffer, brush->points[6], brush->points[7], color);
		tek_shapebuffer_draw_line(buffer, brush->points[7], brush->points[4], color);
		tek_shapebuffer_draw_line(buffer, brush->points[0], brush->points[4], color);
		tek_shapebuffer_draw_line(buffer, brush->points[1], brush->points[5], color);
		tek_shapebuffer_draw_line(buffer, brush->points[2], brush->points[6], color);
		tek_shapebuffer_draw_line(buffer, brush->points[3], brush->points[7], color);
	}
	else if(mode == 2)
	{
		for(u32 i=0; i<6; ++i)
		{
			TekColor color = tek_color_white();
			if(selected == i)
				color = tek_color_red();

			int p0 = brush->planes[i].p0;
			int p1 = brush->planes[i].p1;
			int p2 = brush->planes[i].p2;
			int p3 = brush->planes[i].p3;

			tek_shapebuffer_draw_line(buffer, brush->points[p0], brush->points[p1], color);
			tek_shapebuffer_draw_line(buffer, brush->points[p1], brush->points[p2], color);
			tek_shapebuffer_draw_line(buffer, brush->points[p2], brush->points[p3], color);
			tek_shapebuffer_draw_line(buffer, brush->points[p3], brush->points[p0], color);
		}
	}
	else if(mode == 3)
	{
		for(u32 i=0; i<12; ++i)
		{
			TekColor color = tek_color_white();
			if(selected == i)
				color = tek_color_red();

			int p0 = brush->lines[i].p0;
			int p1 = brush->lines[i].p1;

			tek_shapebuffer_draw_line(buffer, brush->points[p0], brush->points[p1], color);
		}
	}
	else if(mode == 4)
	{
		TekColor color = tek_color_white();
		tek_shapebuffer_draw_line(buffer, brush->points[0], brush->points[1], color);
		tek_shapebuffer_draw_line(buffer, brush->points[1], brush->points[2], color);
		tek_shapebuffer_draw_line(buffer, brush->points[2], brush->points[3], color);
		tek_shapebuffer_draw_line(buffer, brush->points[3], brush->points[0], color);
		tek_shapebuffer_draw_line(buffer, brush->points[4], brush->points[5], color);
		tek_shapebuffer_draw_line(buffer, brush->points[5], brush->points[6], color);
		tek_shapebuffer_draw_line(buffer, brush->points[6], brush->points[7], color);
		tek_shapebuffer_draw_line(buffer, brush->points[7], brush->points[4], color);
		tek_shapebuffer_draw_line(buffer, brush->points[0], brush->points[4], color);
		tek_shapebuffer_draw_line(buffer, brush->points[1], brush->points[5], color);
		tek_shapebuffer_draw_line(buffer, brush->points[2], brush->points[6], color);
		tek_shapebuffer_draw_line(buffer, brush->points[3], brush->points[7], color);

		float offset = 0.05f;
		Vec3 p_original = brush->points[selected];
		Vec3 p0 = vec3_create(p_original.x-offset, p_original.y+offset, p_original.z-offset);
		Vec3 p1 = vec3_create(p_original.x-offset, p_original.y+offset, p_original.z+offset);
		Vec3 p2 = vec3_create(p_original.x+offset, p_original.y+offset, p_original.z+offset);
		Vec3 p3 = vec3_create(p_original.x+offset, p_original.y+offset, p_original.z-offset);

		Vec3 p4 = vec3_create(p_original.x-offset, p_original.y-offset, p_original.z-offset);
		Vec3 p5 = vec3_create(p_original.x-offset, p_original.y-offset, p_original.z+offset);
		Vec3 p6 = vec3_create(p_original.x+offset, p_original.y-offset, p_original.z+offset);
		Vec3 p7 = vec3_create(p_original.x+offset, p_original.y-offset, p_original.z-offset);

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
}

static void update_vertices(TekBrush* brush)
{
	for(u32 i=0; i< 6; ++i)
	{
		Vec3 p0 = brush->points[brush->planes[i].p0];
		Vec3 p1 = brush->points[brush->planes[i].p1];
		Vec3 p2 = brush->points[brush->planes[i].p2];
		Vec3 p3 = brush->points[brush->planes[i].p3];

		Vec2 uv0 = brush->planes[i].uv0;
		Vec2 uv1 = brush->planes[i].uv1;
		Vec2 uv2 = brush->planes[i].uv2;
		Vec2 uv3 = brush->planes[i].uv3;

		brush->planes[i].vertices[0] = TekMeshVertexData(p0,uv0,Vec3());
		brush->planes[i].vertices[1] = TekMeshVertexData(p1,uv1,Vec3());
		brush->planes[i].vertices[2] = TekMeshVertexData(p2,uv2,Vec3());
		brush->planes[i].vertices[3] = TekMeshVertexData(p0,uv0,Vec3());
		brush->planes[i].vertices[4] = TekMeshVertexData(p2,uv2,Vec3());
		brush->planes[i].vertices[5] = TekMeshVertexData(p3,uv3,Vec3());
	}
}

void tek_brush_move_uv(TekBrush* brush, TekPlane* plane, Vec2 v)
{
	tek_plane_move_uv(plane,v);
	update_vertices(brush);
}
void tek_brush_scale_uv(TekBrush* brush, TekPlane* plane,Vec2 v)
{
	tek_plane_scale_uv(plane,v);
	update_vertices(brush);
}

void tek_brush_reset_uv(TekBrush* brush, TekPlane* plane)
{
	tek_plane_reset_uv(plane);
	update_vertices(brush);
}

void tek_brush_move(TekBrush* brush, float x, float y, float z)
{
	brush->points[0].x += x;
	brush->points[0].y += y;
	brush->points[0].z += z;

	brush->points[1].x += x;
	brush->points[1].y += y;
	brush->points[1].z += z;

	brush->points[2].x += x;
	brush->points[2].y += y;
	brush->points[2].z += z;

	brush->points[3].x += x;
	brush->points[3].y += y;
	brush->points[3].z += z;

	brush->points[4].x += x;
	brush->points[4].y += y;
	brush->points[4].z += z;

	brush->points[5].x += x;
	brush->points[5].y += y;
	brush->points[5].z += z;

	brush->points[6].x += x;
	brush->points[6].y += y;
	brush->points[6].z += z;

	brush->points[7].x += x;
	brush->points[7].y += y;
	brush->points[7].z += z;

	update_vertices(brush);
}

void tek_brush_move_point(TekBrush* brush, int point, float x, float y, float z)
{
	brush->points[point].x += x;
	brush->points[point].y += y;
	brush->points[point].z += z;

	update_vertices(brush);
}

void tek_brush_move_line(TekBrush* brush, int line, float x, float y, float z)
{
	brush->points[brush->lines[line].p0].x += x;
	brush->points[brush->lines[line].p0].y += y;
	brush->points[brush->lines[line].p0].z += z;

	brush->points[brush->lines[line].p1].x += x;
	brush->points[brush->lines[line].p1].y += y;
	brush->points[brush->lines[line].p1].z += z;

	update_vertices(brush);
}

void tek_brush_move_plane(TekBrush* brush, int plane, float x, float y, float z)
{
	brush->points[brush->planes[plane].p0].x += x;
	brush->points[brush->planes[plane].p0].y += y;
	brush->points[brush->planes[plane].p0].z += z;

	brush->points[brush->planes[plane].p1].x += x;
	brush->points[brush->planes[plane].p1].y += y;
	brush->points[brush->planes[plane].p1].z += z;

	brush->points[brush->planes[plane].p2].x += x;
	brush->points[brush->planes[plane].p2].y += y;
	brush->points[brush->planes[plane].p2].z += z;

	brush->points[brush->planes[plane].p3].x += x;
	brush->points[brush->planes[plane].p3].y += y;
	brush->points[brush->planes[plane].p3].z += z;

	update_vertices(brush);
}
