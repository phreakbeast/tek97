#ifndef _TEK_INPUT_HPP
#define _TEK_INPUT_HPP

#include "../core/tek_core.hpp"
#include "tek_platform.hpp"
#include "tek_iwindow.hpp"

namespace tek
{
	struct TekKeyboardState
	{
		bool keys_down[NUM_KEYS];
		bool keys_pressed[NUM_KEYS];
		bool keys_released[NUM_KEYS];

		TekInputLetter *letters;
		u32 num_letters;
	};

	struct TekMouseState
	{
		int mx;
		int my;
		int old_mx;
		int old_my;
		int wheel;

		bool buttons_down[NUM_MOUSE_BUTTONS];
		bool buttons_pressed[NUM_MOUSE_BUTTONS];
		bool buttons_released[NUM_MOUSE_BUTTONS];
	};

	class Input
	{
	public:
		Input(IWindow* window);
		~Input();

		bool init();

		void update();

		TekMouseState* get_mouse_state(){return &mouse_state;}
		TekKeyboardState* get_key_state(){return &key_state;}
	private:
		IWindow* window;
		TekKeyboardState key_state;
		TekMouseState mouse_state;
	};
}
#endif
