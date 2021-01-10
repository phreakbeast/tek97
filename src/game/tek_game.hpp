#ifndef _TEK_GAME_HPP
#define _TEK_GAME_HPP

#include "../core/tek_core.hpp"
#include "../tek_application.hpp"
#include "tek_map.hpp"

namespace game
{
	class Game : public tek::Application
	{
	public:
		Game();

		~Game();

		bool on_init() override;

		void on_render() override;

		void on_update(float delta) override;

		void on_resize(u32 width, u32 height) override;
	private:
		tek::TekFont* font;
		Map* map;
		tek::Vec2 cam_pos;
	};

}
#endif
