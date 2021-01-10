#ifndef _TEK_MAP_HPP
#define _TEK_MAP_HPP

#include "../drawing/tek_drawing.hpp"
#include "tek_terrain.hpp"

namespace game
{
	class Map
	{
	public:
		Map();
		~Map();
		void render(tek::Renderer* renderer, tek::Vec2 cam_pos);
	private:
		Terrain* terrain;
	};
}

#endif