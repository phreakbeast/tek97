#ifndef _TEK_BILLBOARD_HPP
#define _TEK_BILLBOARD_HPP

#include "tek_rect.hpp"
#include "../core/tek_core.hpp"
#include "tek_mesh.hpp"

class TekBillboard
{
public:
	TekMeshVertexData vertices[6];
	TekBillboard(float width, float height, TekRect uv);
	~TekBillboard();
};

#endif
