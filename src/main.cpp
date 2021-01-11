
#include "tek_application.hpp"
#include "drawing/tek_rect.hpp"
#include "drawing/tek_drawing.hpp"
#include "game/tek_game.hpp"

static bool on_init()
{
    if(!game_init())
	return false;
    
    return true;
}

static void on_render()
{
    game_render();
}

static void on_update(float delta)
{
    game_update(delta);
}

static void on_resize(u32 width, u32 height)
{
    game_resize(width, height);
}

int main(int argc, char *args[])
{
    int width = 1280;
    int height = 720;
    int render_width = 1280;
    int render_height = 720;

    app_set_init_callback(on_init);
    app_set_render_callback(on_render);
    app_set_update_callback(on_update);
    app_set_resize_callback(on_resize);
    
    if (!app_init(width, height, render_width, render_height, "tek97"))
    {
	return 1;
    }

    app_start_main_loop();

    app_destroy();
    return 0;
}
