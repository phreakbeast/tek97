
#include "tek_application.hpp"
#include "drawing/tek_rect.hpp"
#include "drawing/tek_drawing.hpp"
#include "game/tek_game.hpp"


int main(int argc, char *args[])
{
	int width = 1280;
	int height = 720;
	int render_width = 1280;
	int render_height = 720;

	game::Game game;
	if (!game.init(width, height, render_width, render_height, "tek97"))
	{
		return 1;
	}

	game.start_main_loop();

	return 0;
}
