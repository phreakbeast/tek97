#ifndef _TEK_IWINDOW_HPP
#define _TEK_IWINDOW_HPP

#include "../core/tek_core.hpp"

bool window_open(u32 width, u32 height, const char *title, bool fullscreen);

void window_destroy();
    
void window_toggle_fullscreen();

void window_swap_buffers();

bool window_update();

bool window_should_close();

bool window_is_key_down(Key key);

bool window_is_mouse_down(MouseButton button);

int window_get_mouse_x();

int window_get_mouse_y();

int window_get_mouse_wheel();

void window_set_cursor_pos(int x, int y);

void window_show_cursor(int val);

void window_resize_callback(void(*f)(u32,u32));

#endif
