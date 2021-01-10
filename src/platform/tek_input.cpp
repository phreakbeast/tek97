#include "tek_input.hpp"

namespace tek
{
	Input::Input(IWindow* window)
	{
		this->window = window;
	}

	Input::~Input()
	{

	}

	bool Input::init()
	{
		for (u32 i = 0; i < NUM_KEYS; ++i)
		{
			key_state.keys_down[i] = 0;
			key_state.keys_pressed[i] = 0;
			key_state.keys_released[i] = 0;
		}

		mouse_state.mx = 0;
		mouse_state.my = 0;
		mouse_state.old_mx = 0;
		mouse_state.old_my = 0;
		mouse_state.wheel = 0;
		for (u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			mouse_state.buttons_down[i] = 0;
			mouse_state.buttons_pressed[i] = 0;
			mouse_state.buttons_released[i] = 0;
		}

		return true;
	}

	void Input::update()
	{
		for (u32 i = 0; i < NUM_KEYS; ++i)
		{
			int down = window->is_key_down((Key) i);

			if (down)
			{

				if (key_state.keys_down[i])
				{
					key_state.keys_pressed[i] = 0;
				}
				else
				{
					key_state.keys_pressed[i] = 1;
				}
				key_state.keys_down[i] = 1;
				key_state.keys_released[i] = 0;
			}
			else
			{
				if (key_state.keys_down[i])
				{
					key_state.keys_released[i] = 1;
				}
				else
				{
					key_state.keys_released[i] = 0;
				}
				key_state.keys_down[i] = 0;
				key_state.keys_pressed[i] = 0;
			}

		}

		mouse_state.old_mx = mouse_state.mx;
		mouse_state.old_my = mouse_state.my;

		mouse_state.mx = window->get_mouse_x();
		mouse_state.my = window->get_mouse_y();
		mouse_state.wheel = window->get_mouse_wheel();

		for (u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			int down = window->is_mouse_down((MouseButton) i);

			if (down)
			{
				if (mouse_state.buttons_down[i])
				{
					mouse_state.buttons_pressed[i] = 0;
				}
				else
				{
					mouse_state.buttons_pressed[i] = 1;
				}
				mouse_state.buttons_down[i] = 1;
				mouse_state.buttons_released[i] = 0;
			}
			else
			{
				if (mouse_state.buttons_down[i])
				{
					mouse_state.buttons_released[i] = 1;
				}
				else
				{
					mouse_state.buttons_released[i] = 0;
				}
				mouse_state.buttons_down[i] = 0;
				mouse_state.buttons_pressed[i] = 0;
			}
		}

	}
}
