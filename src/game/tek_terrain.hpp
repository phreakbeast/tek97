#ifndef _TEK_TERRAIN_HPP
#define _TEK_TERRAIN_HPP

#include "../core/tek_core.hpp"
#include "../drawing/tek_drawing.hpp"

namespace game
{
	class Tile
	{
	public:
		enum class Type
		{
			Gras
		};

		Type type;
	};

	class Terrain
	{
	public:
		Terrain(u32 width, u32 height);
		~Terrain();
		void render(tek::Renderer* renderer, tek::Vec2 cam_pos);
	private:
		u32 width;
		u32 height;
		Tile* tiles;
		u32 num_tiles;
	};
}

#endif