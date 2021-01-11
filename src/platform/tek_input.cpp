#include "tek_input.hpp"

#include "tek_iwindow.hpp"

static TekKeyboardState g_key_state;
static TekMouseState g_mouse_state;

bool input_init()
{
    for (u32 i = 0; i < NUM_KEYS; ++i)
    {
	g_key_state.keys_down[i] = 0;
	g_key_state.keys_pressed[i] = 0;
	g_key_state.keys_released[i] = 0;
    }

    g_mouse_state.mx = 0;
    g_mouse_state.my = 0;
    g_mouse_state.old_mx = 0;
    g_mouse_state.old_my = 0;
    g_mouse_state.wheel = 0;
    for (u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
	g_mouse_state.buttons_down[i] = 0;
	g_mouse_state.buttons_pressed[i] = 0;
	g_mouse_state.buttons_released[i] = 0;
    }

    return true;
}

void input_destroy()
{
	
}
    
void input_update()
{
    for (u32 i = 0; i < NUM_KEYS; ++i)
    {
	int down = window_is_key_down((Key) i);

	if (down)
	{

	    if (g_key_state.keys_down[i])
	    {
		g_key_state.keys_pressed[i] = 0;
	    }
	    else
	    {
		g_key_state.keys_pressed[i] = 1;
	    }
	    g_key_state.keys_down[i] = 1;
	    g_key_state.keys_released[i] = 0;
	}
	else
	{
	    if (g_key_state.keys_down[i])
	    {
		g_key_state.keys_released[i] = 1;
	    }
	    else
	    {
		g_key_state.keys_released[i] = 0;
	    }
	    g_key_state.keys_down[i] = 0;
	    g_key_state.keys_pressed[i] = 0;
	}

    }

    g_mouse_state.old_mx = g_mouse_state.mx;
    g_mouse_state.old_my = g_mouse_state.my;

    g_mouse_state.mx = window_get_mouse_x();
    g_mouse_state.my = window_get_mouse_y();
    g_mouse_state.wheel = window_get_mouse_wheel();

    for (u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
	int down = window_is_mouse_down((MouseButton) i);

	if (down)
	{
	    if (g_mouse_state.buttons_down[i])
	    {
		g_mouse_state.buttons_pressed[i] = 0;
	    }
	    else
	    {
		g_mouse_state.buttons_pressed[i] = 1;
	    }
	    g_mouse_state.buttons_down[i] = 1;
	    g_mouse_state.buttons_released[i] = 0;
	}
	else
	{
	    if (g_mouse_state.buttons_down[i])
	    {
		g_mouse_state.buttons_released[i] = 1;
	    }
	    else
	    {
		g_mouse_state.buttons_released[i] = 0;
	    }
	    g_mouse_state.buttons_down[i] = 0;
	    g_mouse_state.buttons_pressed[i] = 0;
	}
    }

}

TekMouseState* input_get_mouse_state()
{
    return &g_mouse_state;
}
    
TekKeyboardState* input_get_key_state()
{
    return &g_key_state;
}
