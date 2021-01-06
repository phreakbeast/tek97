#ifndef _TEK_APPLICATION_HPP
#define _TEK_APPLICATION_HPP

#include "core/tek_core.hpp"

bool tek_app_init(u32 width, u32 height, u32 render_width, u32 render_height, const char* title);
void tek_app_destroy();
void tek_app_start_main_loop();

void tek_app_set_on_init(bool(*f)(void));
void tek_app_set_on_render(void(*f)(void));
void tek_app_set_on_update(void(*f)(float));
void tek_app_set_on_resize(void(*f)(u32,u32));

void tek_app_get_size(u32* w, u32* h);

#endif
