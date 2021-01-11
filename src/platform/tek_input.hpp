#ifndef _TEK_INPUT_HPP
#define _TEK_INPUT_HPP

#include "../core/tek_core.hpp"
#include "tek_platform.hpp"

typedef struct
{
    bool keys_down[NUM_KEYS];
    bool keys_pressed[NUM_KEYS];
    bool keys_released[NUM_KEYS];

    TekInputLetter *letters;
    u32 num_letters;
}TekKeyboardState;

typedef struct
{
    int mx;
    int my;
    int old_mx;
    int old_my;
    int wheel;

    bool buttons_down[NUM_MOUSE_BUTTONS];
    bool buttons_pressed[NUM_MOUSE_BUTTONS];
    bool buttons_released[NUM_MOUSE_BUTTONS];
}TekMouseState;

bool input_init();
void input_destroy();
void input_update();

TekMouseState* input_get_mouse_state();
TekKeyboardState* input_get_key_state();

#endif
