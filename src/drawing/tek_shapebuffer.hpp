#ifndef _TEK_SHAPEBUFFER_HPP
#define _TEK_SHAPEBUFFER_HPP

#include "../math/tek_math.hpp"
#include "tek_color.hpp"
#include "tek_camera.hpp"
#include <vector>

typedef struct
{
	Vec3 position;
	Vec3 color;
}TekSBufferVertexData;

typedef struct
{
	TekCamera* cam;
	std::vector<TekSBufferVertexData> buffer;
}TekShapebuffer;

void tek_shapebuffer_init(TekShapebuffer* buffer, TekCamera* cam);
void tek_shapebuffer_destroy(TekShapebuffer* buffer);
void tek_shapebuffer_draw_line(TekShapebuffer* buffer, Vec3 p0, Vec3 p1, TekColor color);
void tek_shapebuffer_render(TekShapebuffer* buffer);
void tek_shapebuffer_reset(TekShapebuffer* buffer);

#endif
