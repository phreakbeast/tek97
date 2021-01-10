#include "tek_map.hpp"

namespace game
{
	Map::Map()
	{
		terrain = new Terrain(256,256);
	}

	Map::~Map()
	{
		delete terrain;
	}

	void Map::render(tek::Renderer* renderer, tek::Vec2 cam_pos)
	{
		terrain->render(renderer,cam_pos);
	}
}