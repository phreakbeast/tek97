#include "tek_map.hpp"

static void update_vertices(TekQuad *quad);

void tek_plane_move_uv(TekPlane* plane, Vec2 v)
{
	plane->uv0 = vec2_add(plane->uv0,v);
	plane->uv1 = vec2_add(plane->uv1,v);
	plane->uv2 = vec2_add(plane->uv2,v);
	plane->uv3 = vec2_add(plane->uv3,v);
}

void tek_plane_scale_uv(TekPlane* plane,Vec2 v)
{
	plane->uv0.y += v.y;
	plane->uv3.y += v.y;

	plane->uv2.x += v.x;
	plane->uv3.x += v.x;
}

void tek_plane_reset_uv(TekPlane* plane)
{
	plane->uv0 = vec2_create(0,1);
	plane->uv1 = vec2_create(0,0);
	plane->uv2 = vec2_create(1,0);
	plane->uv3 = vec2_create(1,1);
}

void tek_quad_init(TekQuad *quad)
{
	quad->points[0] = vec3_create(0, 0, 0);
	quad->points[1] = vec3_create(0, 0, 1);
	quad->points[2] = vec3_create(1, 0, 1);
	quad->points[3] = vec3_create(1, 0, 0);

	quad->lines[0].p0 = 0;
	quad->lines[0].p1 = 3;

	quad->lines[1].p0 = 0;
	quad->lines[1].p1 = 1;

	quad->lines[2].p0 = 1;
	quad->lines[2].p1 = 2;

	quad->lines[3].p0 = 2;
	quad->lines[3].p1 = 3;

	quad->plane.p0 = 0;
	quad->plane.p1 = 1;
	quad->plane.p2 = 2;
	quad->plane.p3 = 3;
	quad->plane.l0 = 0;
	quad->plane.l1 = 1;
	quad->plane.l2 = 2;
	quad->plane.l3 = 3;
	quad->plane.mat_id = -1;

	quad->plane.uv0 = vec2_create(0,1);
	quad->plane.uv1 = vec2_create(0,0);
	quad->plane.uv2 = vec2_create(1,0);
	quad->plane.uv3 = vec2_create(1,1);

	update_vertices(quad);
}

void tek_quad_render(TekQuad *quad, TekMeshbuffer *buffer, TekMap* map)
{
	if (quad->plane.mat_id >= 0)
	{
		TekMaterial* mat = &map->materials[quad->plane.mat_id].material;
		tek_mb_draw_vertices(buffer, quad->plane.vertices, 6, mat);
	}
}

void tek_quad_render_shape(TekQuad *quad, TekShapebuffer *buffer, int mode, int selected)
{
	if(mode == 1)
	{
		TekColor color = tek_color_white();
		tek_shapebuffer_draw_line(buffer, quad->points[0], quad->points[1], color);
		tek_shapebuffer_draw_line(buffer, quad->points[1], quad->points[2], color);
		tek_shapebuffer_draw_line(buffer, quad->points[2], quad->points[3], color);
		tek_shapebuffer_draw_line(buffer, quad->points[3], quad->points[0], color);
	}
	else if(mode == 2)
	{
		for(u32 i=0; i<4; ++i)
		{
			TekColor color = tek_color_white();
			if(selected == i)
				color = tek_color_red();

			int p0 = quad->lines[i].p0;
			int p1 = quad->lines[i].p1;

			tek_shapebuffer_draw_line(buffer, quad->points[p0], quad->points[p1], color);
		}
	}
	else if(mode == 3)
	{
		TekColor color = tek_color_white();
		tek_shapebuffer_draw_line(buffer, quad->points[0], quad->points[1], color);
		tek_shapebuffer_draw_line(buffer, quad->points[1], quad->points[2], color);
		tek_shapebuffer_draw_line(buffer, quad->points[2], quad->points[3], color);
		tek_shapebuffer_draw_line(buffer, quad->points[3], quad->points[0], color);

		float offset = 0.05f;
		Vec3 p_original = quad->points[selected];
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

void tek_quad_move(TekQuad *quad, float x, float y, float z)
{
	quad->points[0].x += x;
	quad->points[0].y += y;
	quad->points[0].z += z;

	quad->points[1].x += x;
	quad->points[1].y += y;
	quad->points[1].z += z;

	quad->points[2].x += x;
	quad->points[2].y += y;
	quad->points[2].z += z;

	quad->points[3].x += x;
	quad->points[3].y += y;
	quad->points[3].z += z;

	update_vertices(quad);
}

void tek_quad_move_point(TekQuad *quad, int point, float x, float y, float z)
{
	quad->points[point].x += x;
	quad->points[point].y += y;
	quad->points[point].z += z;

	update_vertices(quad);
}

void tek_quad_move_line(TekQuad *quad, int line, float x, float y, float z)
{
	quad->points[quad->lines[line].p0].x += x;
	quad->points[quad->lines[line].p0].y += y;
	quad->points[quad->lines[line].p0].z += z;

	quad->points[quad->lines[line].p1].x += x;
	quad->points[quad->lines[line].p1].y += y;
	quad->points[quad->lines[line].p1].z += z;

	update_vertices(quad);
}

void tek_quad_move_uv(TekQuad* quad, TekPlane* plane, Vec2 v)
{
	tek_plane_move_uv(plane,v);
	update_vertices(quad);
}

void tek_quad_scale_uv(TekQuad* quad, TekPlane* plane,Vec2 v)
{
	tek_plane_scale_uv(plane,v);
	update_vertices(quad);
}

void tek_quad_reset_uv(TekQuad* quad, TekPlane* plane)
{
	tek_plane_reset_uv(plane);
	update_vertices(quad);
}

static void update_vertices(TekQuad *quad)
{
	Vec3 p0 = quad->points[quad->plane.p0];
	Vec3 p1 = quad->points[quad->plane.p1];
	Vec3 p2 = quad->points[quad->plane.p2];
	Vec3 p3 = quad->points[quad->plane.p3];
	Vec2 uv0 = quad->plane.uv0;
	Vec2 uv1 = quad->plane.uv1;
	Vec2 uv2 = quad->plane.uv2;
	Vec2 uv3 = quad->plane.uv3;

	quad->plane.vertices[0] = TekMeshVertexData(p0, uv0, vec3_create(0,0,0));
	quad->plane.vertices[1] = TekMeshVertexData(p1, uv1, vec3_create(0,0,0));
	quad->plane.vertices[2] = TekMeshVertexData(p2, uv2, vec3_create(0,0,0));
	quad->plane.vertices[3] = TekMeshVertexData(p0, uv0, vec3_create(0,0,0));
	quad->plane.vertices[4] = TekMeshVertexData(p2, uv2, vec3_create(0,0,0));
	quad->plane.vertices[5] = TekMeshVertexData(p3, uv3, vec3_create(0,0,0));
}