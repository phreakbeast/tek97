#include "tek_game.hpp"

#include "../drawing/tek_drawing.hpp"
#include "../core/tek_input.hpp"
#include "../drawing/tek_font.hpp"

static tek::TekFont* g_font;

bool tek_game_init(u32 width, u32 height)
{
    g_font = tek::TekFont::load("data/fonts/test.font");
    if(!g_font)
	return false;

    return true;
}

void tek_game_destroy()
{
    delete g_font;
}

void tek_game_render()
{
    tek::tek_renderer_disable_depth_test();
    //render 2d
    tek::TekSpritebatch *sb = tek::tek_renderer_get_sb();

    //render gui
    tek::tek_sb_begin(sb);

    //render debug text
    tek::TekRenderStats *stats = tek::tek_renderer_get_stats();
    char fps_str[256];

    sprintf(fps_str, "fps: %u ups: %u", stats->fps, stats->ups);

    tek_sb_render_text(sb, fps_str, g_font, 5, 5, tek::TekColor::white(), 0);

    tek::tek_sb_end(sb);
    tek::tek_sb_flush(sb);
    tek::tek_renderer_enable_depth_test();
}

void tek_game_update(float delta)
{
    tek::TekKeyboardState *kstate = tek::tek_input_get_key_state();
}
