#ifndef _TEK_INPUT_HPP
#define _TEK_INPUT_HPP

#include "tek_core.hpp"
#include "../platform/tek_platform.hpp"

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


    bool tek_input_init();

    void tek_input_update();

    TekMouseState* tek_input_get_mouse_state();

    TekKeyboardState* tek_input_get_key_state();
}
#endif
