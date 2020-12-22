#ifndef _TEK_CONSOLE_HPP
#define _TEK_CONSOLE_HPP

#include "../core/tek_core.hpp"
#include "../drawing/tek_spritebatch.hpp"

bool tek_console_init();
void tek_console_destroy();
void tek_console_update(float delta);
void tek_console_render(TekSpritebatch* sb);
void tek_console_toggle();
bool tek_console_active();

#endif