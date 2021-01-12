#include "tek_game.hpp"

#include "../drawing/tek_drawing.hpp"
#include "../platform/tek_input.hpp"
#include "../drawing/tek_font.hpp"
#include "tek_map.hpp"

static TekFont g_font;
static Map g_map;

static TekCamera* g_cam = nullptr;

void game_destroy()
{
    delete g_cam;
    map_destroy(&g_map);
    tek_font_destroy(&g_font);
    tek_free(&g_font);
}

bool game_init()
{
    if(!tek_font_load(&g_font,"data/fonts/test.font"))
	return false;

    map_init(&g_map);

    Mat4 projection = mat4_perspective(45.0f, 1280.0f/720.0f, 0.1f, 100.0f);
    //projection = mat4_transposed(&projection);
    g_cam = new TekCamera(TekCamera::Type::Third,projection);
    
    return true;
}

void game_render()
{
    //render 3d
    map_render(&g_map, g_cam);
    
    renderer_disable_depth_test();
    //render 2d
    TekSpritebatch *sb = renderer_get_sb();
    tek_sb_begin(sb);


    //render debug text
    TekRenderStats *stats = renderer_get_stats();
    char fps_str[256];

    sprintf(fps_str, "fps: %u ups: %u", stats->fps, stats->ups);

    tek_sb_render_text(sb, fps_str, &g_font, 5, 5, Color::white(), 0);

    tek_sb_end(sb);
    tek_sb_flush(sb);
    renderer_enable_depth_test();
}

void game_update(float delta)
{
    TekKeyboardState *kstate = input_get_key_state();
    if(kstate->keys_down[KEY_A])
    {
    }
    if(kstate->keys_down[KEY_D])
    {
    }
}

void game_resize(u32 width, u32 height)
{

}

