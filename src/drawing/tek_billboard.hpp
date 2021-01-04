#ifndef _TEK_BILLBOARD_HPP
#define _TEK_BILLBOARD_HPP

#include "tek_rect.hpp"
#include "../core/tek_core.hpp"
#include "tek_mesh.hpp"

typedef struct
{
	TekMeshVertexData vertices[6];
}TekBillboard;

void tek_bb_init(TekBillboard* bb,float width, float height, TekRect uv);

#endif
