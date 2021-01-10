#include "tek_terrain.hpp"

namespace game
{
	Terrain::Terrain(u32 width, u32 height)
	{
		this->width = width;
		this->height = height;

		num_tiles = width * height;
		tiles = new Tile[num_tiles]();
	}

	Terrain::~Terrain()
	{
		delete tiles;
	}

	void Terrain::render(tek::Renderer* renderer, tek::Vec2 cam_pos)
	{
		for(u32 y=0; y<height;++y)
		{
			for(u32 x=0; x<width;++x)
			{
				tek::TekSpritebatch* sb = renderer->get_sb();

				sb->render_rect(tek::TekRect(x*32,y*32,32,32),tek::TekColor::random());
			}
		}
	}
}