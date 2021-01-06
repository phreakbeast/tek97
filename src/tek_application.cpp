#include "tek_application.hpp"

#include "drawing/tek_drawing.hpp"
#include "platform/tek_platform.hpp"
#include "core/tek_input.hpp"

static bool g_is_running;

static float g_update_timer;
static float g_update_ticks;
static float g_timer;

static u32 g_frames;
static u32 g_updates;
static u32 g_fps;
static u32 g_ups;

static float g_delta;

static tek::TekStopwatch g_ups_timer;
static tek::TekStopwatch g_delta_timer;

static u32 g_width = 0;
static u32 g_height = 0;

bool (*on_init_callback)(void) = NULL;
void (*on_render_callback)(void) = NULL;
void (*on_update_callback)(float) = NULL;
void (*on_resize_callback)(u32,u32) = NULL;

void app_on_resize(u32 width, u32 height)
{
    if(on_resize_callback)
    {
	on_resize_callback(width,height);
    }
}

void tek_app_destroy()
{
    tek::tek_renderer_destroy();
    tek_window_destroy();
}

bool tek_app_init(u32 width, u32 height, u32 render_width, u32 render_height, const char* title)
{
    g_width = width;
    g_height = height;

    g_is_running = true;

    g_update_timer = 0;
    g_update_ticks = 1.0f / 60.0f;
    g_timer = 0;

    g_frames = 0;
    g_updates = 0;
    g_fps = 0;
    g_ups = 0;

    g_delta = 0;

    tek_window_set_on_resize(app_on_resize);

    if (!tek_window_open(g_width, g_height, title, 0))
	return false;

    if (!tek::tek_input_init())
	return false;

    if (!tek::tek_renderer_init(render_width, render_height))
	return false;

    tek_time_init();

    //run on_init callback
    bool result = true;
    if(on_init_callback != NULL)
    {
	result = on_init_callback();
    }
    return result;
}

void tek_app_start_main_loop()
{
    g_ups_timer.start();
    g_delta_timer.start();

    while (g_is_running)
    {
	tek::TekRenderStats *stats = tek::tek_renderer_get_stats();
	double seconds = g_ups_timer.get_seconds();
	if (seconds - g_update_timer > g_update_ticks)
	{
	    g_delta = (float) g_delta_timer.get_seconds();
	    g_delta_timer.start();
	    if (!tek_window_update())
	    {
		break;
	    }
	    tek::tek_input_update();

	    tek::TekKeyboardState *kstate = tek::tek_input_get_key_state();
	    if (kstate->keys_pressed[KEY_ESCAPE])
	    {
		break;
	    }

	    g_is_running = !tek_window_should_close();

	    if(on_update_callback != NULL)
	    {
		on_update_callback(g_delta);
	    }

	    g_update_timer += g_update_ticks;
	    g_updates++;
	}

	tek::tek_renderer_start_frame();
	if(on_render_callback != NULL)
	{
	    on_render_callback();
	}


	tek::tek_renderer_end_frame();

	tek_window_swap_buffers();
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

void tek_app_set_on_init(bool(*f)(void))
{
    on_init_callback = f;
}

void tek_app_set_on_render(void(*f)(void))
{
    on_render_callback = f;
}

void tek_app_set_on_update(void(*f)(float))
{
    on_update_callback = f;
}

void tek_app_set_on_resize(void(*f)(u32,u32))
{
    on_resize_callback = f;
}

void tek_app_get_size(u32* w, u32* h)
{
    if(w)
    {
	*w = g_width;
    }

    if(h)
    {
	*h = g_height;
    }
}
