#include "tek_map.hpp"

#include "../drawing/tek_drawing.hpp"

TekBrush::TekBrush()
{
	points[0] = Vec3(0,1,0);
	points[1] = Vec3(0,1,1);
	points[2] = Vec3(1,1,1);
	points[3] = Vec3(1,1,0);
	points[4] = Vec3(0,0,0);
	points[5] = Vec3(0,0,1);
	points[6] = Vec3(1,0,1);
	points[7] = Vec3(1,0,0);

	lines[0].p0 = 0;
	lines[0].p1 = 3;

	lines[1].p0 = 0;
	lines[1].p1 = 1;

	lines[2].p0 = 1;
	lines[2].p1 = 2;

	lines[3].p0 = 2;
	lines[3].p1 = 3;

	lines[4].p0 = 4;
	lines[4].p1 = 7;

	lines[5].p0 = 4;
	lines[5].p1 = 5;

	lines[6].p0 = 5;
	lines[6].p1 = 6;

	lines[7].p0 = 6;
	lines[7].p1 = 7;

	lines[8].p0 = 0;
	lines[8].p1 = 4;

	lines[9].p0 = 1;
	lines[9].p1 = 5;

	lines[10].p0 = 2;
	lines[10].p1 = 6;

	lines[11].p0 = 3;
	lines[11].p1 = 7;

	//front plane
	planes[0].p0 = 1;
	planes[0].p1 = 5;
	planes[0].p2 = 6;
	planes[0].p3 = 2;
	planes[0].l0 = 5;
	planes[0].l1 = 9;
	planes[0].l2 = 6;
	planes[0].l3 = 10;

	//left plane
	planes[1].p0 = 0;
	planes[1].p1 = 4;
	planes[1].p2 = 5;
	planes[1].p3 = 1;
	planes[1].l0 = 1;
	planes[1].l1 = 8;
	planes[1].l2 = 5;
	planes[1].l3 = 9;

	//right plane
	planes[2].p0 = 2;
	planes[2].p1 = 6;
	planes[2].p2 = 7;
	planes[2].p3 = 3;
	planes[2].l0 = 3;
	planes[2].l1 = 10;
	planes[2].l2 = 7;
	planes[2].l3 = 11;

	//back plane
	planes[3].p0 = 3;
	planes[3].p1 = 7;
	planes[3].p2 = 4;
	planes[3].p3 = 0;
	planes[3].l0 = 0;
	planes[3].l1 = 11;
	planes[3].l2 = 4;
	planes[3].l3 = 8;

	//top plane
	planes[4].p0 = 0;
	planes[4].p1 = 1;
	planes[4].p2 = 2;
	planes[4].p3 = 3;
	planes[4].l0 = 0;
	planes[4].l1 = 1;
	planes[4].l2 = 2;
	planes[4].l3 = 3;

	//bottom plane
	planes[5].p0 = 6;
	planes[5].p1 = 5;
	planes[5].p2 = 4;
	planes[5].p3 = 7;
	planes[5].l0 = 6;
	planes[5].l1 = 5;
	planes[5].l2 = 4;
	planes[5].l3 = 7;

	planes[0].mat = nullptr;
	planes[1].mat = nullptr;
	planes[2].mat = nullptr;
	planes[3].mat = nullptr;
	planes[4].mat = nullptr;
	planes[5].mat = nullptr;

	planes[0].uv0 = vec2_create(0,1);
	planes[0].uv1 = vec2_create(0,0);
	planes[0].uv2 = vec2_create(1,0);
	planes[0].uv3 = vec2_create(1,1);

	planes[1].uv0 = vec2_create(0,1);
	planes[1].uv1 = vec2_create(0,0);
	planes[1].uv2 = vec2_create(1,0);
	planes[1].uv3 = vec2_create(1,1);

	planes[2].uv0 = vec2_create(0,1);
	planes[2].uv1 = vec2_create(0,0);
	planes[2].uv2 = vec2_create(1,0);
	planes[2].uv3 = vec2_create(1,1);

	planes[3].uv0 = vec2_create(0,1);
	planes[3].uv1 = vec2_create(0,0);
	planes[3].uv2 = vec2_create(1,0);
	planes[3].uv3 = vec2_create(1,1);

	planes[4].uv0 = vec2_create(0,1);
	planes[4].uv1 = vec2_create(0,0);
	planes[4].uv2 = vec2_create(1,0);
	planes[4].uv3 = vec2_create(1,1);

	planes[5].uv0 = vec2_create(0,1);
	planes[5].uv1 = vec2_create(0,0);
	planes[5].uv2 = vec2_create(1,0);
	planes[5].uv3 = vec2_create(1,1);

	update_vertices();
}

void TekBrush::render(TekMeshbuffer* buffer)
{
	for(u32 i=0; i< 6; ++i)
	{
		if(planes[i].mat)
		{
			buffer->draw_vertices(planes[i].vertices,6,planes[i].mat);
		}
	}
}

void TekBrush::render_shape(TekShapebuffer* buffer, int mode, int selected)
{
	if(mode == 1)
	{
		tek_shapebuffer_draw_line(buffer, points[0], points[1], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[1], points[2], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[2], points[3], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[3], points[0], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[4], points[5], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[5], points[6], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[6], points[7], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[7], points[4], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[0], points[4], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[1], points[5], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[2], points[6], TekColor::white());
		tek_shapebuffer_draw_line(buffer, points[3], points[7], TekColor::white());
	}
	else if(mode == 2)
	{
		for(u32 i=0; i<6; ++i)
		{
			TekColor color = TekColor::white();
			if(selected == i)
				color = TekColor::red();

			int p0 = planes[i].p0;
			int p1 = planes[i].p1;
			int p2 = planes[i].p2;
			int p3 = planes[i].p3;

			tek_shapebuffer_draw_line(buffer, points[p0], points[p1], color);
			tek_shapebuffer_draw_line(buffer, points[p1], points[p2], color);
			tek_shapebuffer_draw_line(buffer, points[p2], points[p3], color);
			tek_shapebuffer_draw_line(buffer, points[p3], points[p0], color);
		}
	}
	else if(mode == 3)
	{
		for(u32 i=0; i<12; ++i)
		{
			TekColor color = TekColor::white();
			if(selected == i)
				color = TekColor::red();

			int p0 = lines[i].p0;
			int p1 = lines[i].p1;

			tek_shapebuffer_draw_line(buffer, points[p0], points[p1], color);
		}
	}
	else if(mode == 4)
	{
		TekColor color = TekColor::white();
		tek_shapebuffer_draw_line(buffer, points[0], points[1], color);
		tek_shapebuffer_draw_line(buffer, points[1], points[2], color);
		tek_shapebuffer_draw_line(buffer, points[2], points[3], color);
		tek_shapebuffer_draw_line(buffer, points[3], points[0], color);
		tek_shapebuffer_draw_line(buffer, points[4], points[5], color);
		tek_shapebuffer_draw_line(buffer, points[5], points[6], color);
		tek_shapebuffer_draw_line(buffer, points[6], points[7], color);
		tek_shapebuffer_draw_line(buffer, points[7], points[4], color);
		tek_shapebuffer_draw_line(buffer, points[0], points[4], color);
		tek_shapebuffer_draw_line(buffer, points[1], points[5], color);
		tek_shapebuffer_draw_line(buffer, points[2], points[6], color);
		tek_shapebuffer_draw_line(buffer, points[3], points[7], color);

		float offset = 0.05f;
		Vec3 p_original = points[selected];
		Vec3 p0 = Vec3(p_original.x-offset, p_original.y+offset, p_original.z-offset);
		Vec3 p1 = Vec3(p_original.x-offset, p_original.y+offset, p_original.z+offset);
		Vec3 p2 = Vec3(p_original.x+offset, p_original.y+offset, p_original.z+offset);
		Vec3 p3 = Vec3(p_original.x+offset, p_original.y+offset, p_original.z-offset);

		Vec3 p4 = Vec3(p_original.x-offset, p_original.y-offset, p_original.z-offset);
		Vec3 p5 = Vec3(p_original.x-offset, p_original.y-offset, p_original.z+offset);
		Vec3 p6 = Vec3(p_original.x+offset, p_original.y-offset, p_original.z+offset);
		Vec3 p7 = Vec3(p_original.x+offset, p_original.y-offset, p_original.z-offset);

		color = TekColor::red();
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

void TekBrush::update_vertices()
{
	for(u32 i=0; i< 6; ++i)
	{
		Vec3 p0 = points[planes[i].p0];
		Vec3 p1 = points[planes[i].p1];
		Vec3 p2 = points[planes[i].p2];
		Vec3 p3 = points[planes[i].p3];

		Vec2 uv0 = planes[i].uv0;
		Vec2 uv1 = planes[i].uv1;
		Vec2 uv2 = planes[i].uv2;
		Vec2 uv3 = planes[i].uv3;

		planes[i].vertices[0] = TekMeshVertexData(p0,uv0,Vec3());
		planes[i].vertices[1] = TekMeshVertexData(p1,uv1,Vec3());
		planes[i].vertices[2] = TekMeshVertexData(p2,uv2,Vec3());
		planes[i].vertices[3] = TekMeshVertexData(p0,uv0,Vec3());
		planes[i].vertices[4] = TekMeshVertexData(p2,uv2,Vec3());
		planes[i].vertices[5] = TekMeshVertexData(p3,uv3,Vec3());
	}
}

void TekBrush::set_material(TekMaterial* mat, int plane)
{
	planes[plane].mat = mat;
}

void tek_brush_move_uv(TekBrush* brush, TekPlane* plane, Vec2 v)
{
	tek_plane_move_uv(plane,v);
	brush->update_vertices();
}
void tek_brush_scale_uv(TekBrush* brush, TekPlane* plane,Vec2 v)
{
	tek_plane_scale_uv(plane,v);
	brush->update_vertices();
}

void tek_brush_reset_uv(TekBrush* brush, TekPlane* plane)
{
	tek_plane_reset_uv(plane);
	brush->update_vertices();
}

void TekBrush::move(float x, float y, float z)
{
	points[0].x += x;
	points[0].y += y;
	points[0].z += z;

	points[1].x += x;
	points[1].y += y;
	points[1].z += z;

	points[2].x += x;
	points[2].y += y;
	points[2].z += z;

	points[3].x += x;
	points[3].y += y;
	points[3].z += z;

	points[4].x += x;
	points[4].y += y;
	points[4].z += z;

	points[5].x += x;
	points[5].y += y;
	points[5].z += z;

	points[6].x += x;
	points[6].y += y;
	points[6].z += z;

	points[7].x += x;
	points[7].y += y;
	points[7].z += z;

	update_vertices();
}

void TekBrush::move_point(int point, float x, float y, float z)
{
	points[point].x += x;
	points[point].y += y;
	points[point].z += z;

	update_vertices();
}

void TekBrush::move_line(int line, float x, float y, float z)
{
	points[lines[line].p0].x += x;
	points[lines[line].p0].y += y;
	points[lines[line].p0].z += z;

	points[lines[line].p1].x += x;
	points[lines[line].p1].y += y;
	points[lines[line].p1].z += z;

	update_vertices();
}

void TekBrush::move_plane(int plane, float x, float y, float z)
{
	points[planes[plane].p0].x += x;
	points[planes[plane].p0].y += y;
	points[planes[plane].p0].z += z;

	points[planes[plane].p1].x += x;
	points[planes[plane].p1].y += y;
	points[planes[plane].p1].z += z;

	points[planes[plane].p2].x += x;
	points[planes[plane].p2].y += y;
	points[planes[plane].p2].z += z;

	points[planes[plane].p3].x += x;
	points[planes[plane].p3].y += y;
	points[planes[plane].p3].z += z;

	update_vertices();
}
