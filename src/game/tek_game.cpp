#include "tek_game.hpp"

#include "../drawing/tek_drawing.hpp"
#include "../core/tek_input.hpp"

#include "tek_console.hpp"
#include "tek_editor.hpp"

static TekFont *g_font;

bool tek_game_init(u32 width, u32 height)
{
	if(!tek_console_init())
		return false;

	g_font = TekFont::load("data/fonts/test.font");
	if (!g_font)
		return false;

	if(!tek_editor_init(width, height))
		return false;

	return true;
}

void tek_game_destroy()
{
	tek_editor_destroy();
	delete g_font;
	tek_console_destroy();
}

void tek_game_render()
{
	tek_editor_render_3d();

	tek_renderer_disable_depth_test();
	//render 2d
	TekSpritebatch *sb = tek_renderer_get_sb();

	//render gui
	tek_sb_begin(sb);

	tek_editor_render_2d(sb, g_font);
	//render debug text
	TekRenderStats *stats = tek_renderer_get_stats();
	char fps_str[256];

	sprintf(fps_str, "fps: %u ups: %u", stats->fps, stats->ups);

	tek_sb_render_text(sb, fps_str, g_font, 5, 5, TekColor::white(), 0);
	//console
	tek_console_render(sb);

	tek_sb_end(sb);
	tek_sb_flush(sb);
	tek_renderer_enable_depth_test();
}

void tek_game_update(float delta)
{
	TekKeyboardState *kstate = tek_input_get_key_state();
	if(tek_console_active())
	{
		tek_console_update(delta);
	}
	else
	{
		tek_editor_update(delta);
	}

	if (kstate->keys_pressed[KEY_TAB])
	{
		tek_console_toggle();
	}
}