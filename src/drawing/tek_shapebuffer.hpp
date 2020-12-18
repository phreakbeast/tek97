#ifndef _TEK_SHAPEBUFFER_HPP
#define _TEK_SHAPEBUFFER_HPP

#include "../math/tek_math.hpp"
#include "tek_color.hpp"
#include "tek_camera.hpp"
#include <vector>

struct TekShapebuffer
{
	struct VertexData
	{
		Vec3 position;
		Vec3 color;
	};

	TekShapebuffer(TekCamera* cam);
	~TekShapebuffer();
	void draw_line(Vec3 p0, Vec3 p1, TekColor color);
	void render();
	void reset();
private:
	TekCamera* cam;
	std::vector<VertexData> buffer;
};

#endif
