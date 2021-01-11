#ifndef _TEK_APPLICATION_HPP
#define _TEK_APPLICATION_HPP

#include "core/tek_core.hpp"
#include "drawing/tek_rect.hpp"
#include "drawing/tek_framebuffer.hpp"

#include "drawing/tek_drawing.hpp"
#include "platform/tek_iwindow.hpp"
#include "platform/tek_input.hpp"

bool app_init(u32 width, u32 height, u32 render_width, u32 render_height, const char *title);
void app_destroy();
void app_start_main_loop();

void app_set_init_callback(bool(*f)(void));
void app_set_render_callback(void(*f)(void));
void app_set_update_callback(void(*f)(float));
void app_set_resize_callback(void(*f)(u32,u32));

#endif
