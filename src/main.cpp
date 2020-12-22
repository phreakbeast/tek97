

#include "tek_application.hpp"
#include "drawing/tek_rect.hpp"
#include "drawing/tek_drawing.hpp"
#include "game/tek_game.hpp"

static TekRect g_target_rect;
static TekFramebuffer* g_fb;
static Mat4 g_ortho;

static u32 g_width = 0;
static u32 g_height = 0;
static u32 g_render_width = 0;
static u32 g_render_height = 0;

static TekRect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height)
{
	TekRect result;

	if (win_width == render_width && win_height == render_height)
	{
		float x, y, w, h;
		x = 0;
		y = 0;
		w = (float)win_width;
		h = (float)win_height;
		result = TekRect(x, y, w, h);
		return result;
	}

	float ar = (float)win_width / win_height;
	float st_ar = (float)render_width / render_height;

	// screen wider than 16:9
	if (ar > st_ar)
	{
		float new_width = win_height * st_ar;
		float x, y, w, h;

		x = (win_width - new_width) / 2;
		w = new_width;
		y = 0;
		h = (float)win_height;
		result = TekRect(x, y, w, h);
	}
	else
	{
		float x, y, w, h;
		float new_height = win_width / st_ar;
		x = 0;
		w = (float)win_width;

		y = (win_height - new_height) / 2;
		h = new_height;
		result = TekRect(x, y, w, h);
	}

	return result;
}

bool on_init()
{
	if (!tek_game_init(g_render_width,g_render_height))
		return false;

	return true;
}

void on_render()
{	
	tek_renderer_start_frame();
	tek_renderer_bind_framebuffer(g_fb, TekColor::black());

	tek_game_render();

	tek_renderer_unbind_framebuffer();
	tek_renderer_viewport(g_width, g_height);
	g_fb->bind_reading(0);
	tek_renderer_render_sprite(g_target_rect, TekRect(0, 0, 1, 1), g_fb->get_tex_id(),&g_ortho );
}

void on_update(float delta)
{
	tek_game_update(delta);
}

void on_resize(u32 width, u32 height)
{
	g_width = width;
	g_height = height;
	g_target_rect = get_target_rect(width, height, 768, 432);
	g_ortho = mat4_ortho(0, width, 0, height, -1, 1);
	g_ortho = mat4_transposed(&g_ortho);
}

int main(int argc, char* args[])
{
	g_width = 1280;
	g_height = 720;
	g_render_width = 640;
	g_render_height = 360;

	tek_app_set_on_init(on_init);
	tek_app_set_on_render(on_render);
	tek_app_set_on_update(on_update);
	tek_app_set_on_resize(on_resize);

	if (!tek_app_init(g_width, g_height, g_render_width, g_render_height, "tek97"))
	{
		return 1;
	}

	g_target_rect = get_target_rect(g_width, g_height, g_render_width, g_render_height);
	g_ortho = mat4_ortho(0, g_width, 0, g_height, -1, 1);
	g_ortho = mat4_transposed(&g_ortho);
	g_fb = new TekFramebuffer(g_render_width, g_render_height);

	tek_app_start_main_loop();

	tek_game_destroy();
	delete g_fb;
	tek_app_destroy();
	return 0;
}