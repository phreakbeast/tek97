#include "tek_billboard.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_mesh.hpp"
#include "tek_shader.hpp"
#include "tek_drawing.hpp"

void tek_bb_init(TekBillboard* bb, float width, float height, TekRect uv)
{
	float w = width / 2.0f;
	float h = height / 2.0f;

	TekMeshVertexData v0 = { {-w, h, 0},{uv.x, uv.h},{0, 1, 0} };
	TekMeshVertexData v1 = { {-w,-h, 0},{uv.x, uv.y},{0, 1, 0} };
	TekMeshVertexData v2 = { { w,-h, 0},{uv.w, uv.y},{0, 1, 0} };
	TekMeshVertexData v3 = { { w, h, 0},{uv.w, uv.h},{0, 1, 0} };

	bb->vertices[0] = v0;
	bb->vertices[1] = v1;
	bb->vertices[2] = v2;

	bb->vertices[3] = v0;
	bb->vertices[4] = v2;
	bb->vertices[5] = v3;
}