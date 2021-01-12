#include "tek_application.hpp"

#include "drawing/tek_drawing.hpp"
#include "platform/tek_platform.hpp"
#include "platform/tek_input.hpp"
#include "platform/tek_iwindow.hpp"

static TekRect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height);

static bool g_is_running = true;

static float g_update_timer = 0;
static float g_update_ticks = 1.0f / 60.0f;
static float g_timer = 0;

static u32 g_frames = 0;
static u32 g_updates = 0;
static u32 g_fps = 0;
static u32 g_ups = 0;

static float g_delta = 0;

static Stopwatch g_ups_timer;
static Stopwatch g_delta_timer;

static u32 g_width = 0;
static u32 g_height = 0;
static u32 g_render_width = 0;
static u32 g_render_height = 0;

static TekRect g_target_rect;
static TekFramebuffer g_fb;
static Mat4 g_ortho;

static void on_resize(u32 width, u32 height);

static bool (*g_on_init_callback)(void) = nullptr;
static void (*g_on_render_callback)(void) = nullptr;
static void (*g_on_update_callback)(float) = nullptr;
static void (*g_on_resize_callback)(u32,u32) = nullptr;
    
void app_destroy()
{
    tek_fb_destroy(&g_fb);
    renderer_destroy();
    input_destroy();
    window_destroy();
}

bool app_init(u32 width, u32 height, u32 render_width, u32 render_height, const char *title)
{
    g_width = width;
    g_height = height;
    g_render_width = render_width;
    g_render_height = render_height;

    window_resize_callback(on_resize);
	
    if (!window_open(width, height, title, 0))
	return false;

    if (!input_init())
	return false;

    if (!renderer_init(render_width, render_height))
	return false;

    tek_time_init();

    g_target_rect = get_target_rect(width,height,render_width,render_height);
    g_ortho = mat4_ortho(0,width,0,height,-1,1);
    g_ortho = mat4_transposed(&g_ortho);

    tek_fb_init(&g_fb,render_width,render_height);

    bool result = true;
    if(g_on_init_callback)
    {
	result = g_on_init_callback();
    }
    return result;
}

void app_start_main_loop()
{
    g_ups_timer.start();
    g_delta_timer.start();

    while (g_is_running)
    {
	TekRenderStats *stats = renderer_get_stats();
	double seconds = g_ups_timer.get_seconds();
	if (seconds - g_update_timer > g_update_ticks)
	{
	    g_delta = (float)g_delta_timer.get_seconds();
	    g_delta_timer.start();
	    if (!window_update())
	    {
		break;
	    }
	    input_update();

	    TekKeyboardState *kstate = input_get_key_state();
	    if (kstate->keys_pressed[KEY_ESCAPE])
	    {
		break;
	    }

	    g_is_running = !window_should_close();

	    if(g_on_update_callback)
	    {
		g_on_update_callback(g_delta);
	    }

	    g_update_timer += g_update_ticks;
	    g_updates++;
	}

	renderer_start_frame();
	renderer_bind_framebuffer(&g_fb, Color::black());

	if(g_on_render_callback)
	{
	    g_on_render_callback();
	}

	renderer_unbind_framebuffer();
	renderer_viewport(g_width, g_height);
	tek_fb_bind_reading(&g_fb,0);
	renderer_render_sprite(g_target_rect, tek_rect_create(0, 0, 1, 1), g_fb.tex_id, &g_ortho);

	window_swap_buffers();
	g_frames++;

	if (seconds - g_timer > 1.0f)
	{
	    g_timer += 1.0f;
	    //tick;

	    g_fps = g_frames;
	    g_ups = g_updates;
	    g_frames = 0;
	    g_updates = 0;
	    stats->fps = g_fps;
	    stats->ups = g_ups;
	    stats->delta = g_delta;
	}
    }
}

void app_set_init_callback(bool(*f)(void))
{
    g_on_init_callback = f;
}
    
void app_set_render_callback(void(*f)(void))
{
    g_on_render_callback = f;
}
    
void app_set_update_callback(void(*f)(float))
{
    g_on_update_callback = f;
}
    
void app_set_resize_callback(void(*f)(u32,u32))
{
    g_on_resize_callback = f;
}

static void on_resize(u32 width, u32 height)
{
    g_width = width;
    g_height = height;
    g_target_rect = get_target_rect(width,height,g_render_width,g_render_height);
    g_ortho = mat4_ortho(0,width,0,height,-1,1);
    g_ortho = mat4_transposed(&g_ortho);
    if(g_on_render_callback)
    {
	g_on_resize_callback(width,height);
    }
}

static TekRect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height)
{
    TekRect result;

    if (win_width == render_width && win_height == render_height)
    {
	float x, y, w, h;
	x = 0;
	y = 0;
	w = (float) win_width;
	h = (float) win_height;
	result = tek_rect_create(x, y, w, h);
	return result;
    }

    float ar = (float) win_width / win_height;
    float st_ar = (float) render_width / render_height;

    // screen wider than 16:9
    if (ar > st_ar)
    {
	float new_width = win_height * st_ar;
	float x, y, w, h;

	x = (win_width - new_width) / 2;
	w = new_width;
	y = 0;
	h = (float) win_height;
	result = tek_rect_create(x, y, w, h);
    }
    else
    {
	float x, y, w, h;
	float new_height = win_width / st_ar;
	x = 0;
	w = (float) win_width;

	y = (win_height - new_height) / 2;
	h = new_height;
	result = tek_rect_create(x, y, w, h);
    }

    return result;
}

