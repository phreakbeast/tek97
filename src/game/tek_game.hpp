#ifndef _TEK_GAME_HPP
#define _TEK_GAME_HPP

#include "../core/tek_core.hpp"


bool game_init();

void game_destroy();
    
void game_render();

void game_update(float delta);

void game_resize(u32 width, u32 height);


#endif

