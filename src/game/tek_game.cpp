#include "tek_game.hpp"

#include "../drawing/tek_drawing.hpp"
#include "../platform/tek_input.hpp"
#include "../drawing/tek_font.hpp"

namespace game
{
	Game::Game()
	{
		map = nullptr;
		font = nullptr;
	}

	Game::~Game()
	{
		delete font;
		delete map;
	}

	bool Game::on_init()
	{
		font = tek::TekFont::load("data/fonts/test.font");
		if (!font)
			return false;

		map = new Map();

		return true;
	}

	void Game::on_render()
	{
		renderer->disable_depth_test();
		//render 2d
		tek::TekSpritebatch *sb = renderer->get_sb();
		sb->begin();

		map->render(renderer, cam_pos);

		//render debug text
		tek::TekRenderStats *stats = renderer->get_stats();
		char fps_str[256];

		sprintf(fps_str, "fps: %u ups: %u", stats->fps, stats->ups);

		sb->render_text(fps_str, font, 5, 5, tek::TekColor::white(), 0);

		sb->end();
		sb->flush();
		renderer->enable_depth_test();
	}

	void Game::on_update(float delta)
	{
		tek::TekKeyboardState *kstate = input->get_key_state();
		if(kstate->keys_down[tek::KEY_A])
		{
			cam_pos.x -= delta * 15;
		}
		if(kstate->keys_down[tek::KEY_D])
		{
			cam_pos.x += delta * 15;
		}
	}

	void Game::on_resize(u32 width, u32 height)
	{

	}
}