#ifndef _TEK_GAME_HPP
#define _TEK_GAME_HPP

#include "../core/tek_core.hpp"

bool tek_game_init(u32 width, u32 height);
void tek_game_destroy();
void tek_game_render();
void tek_game_update(float delta);

#endif
