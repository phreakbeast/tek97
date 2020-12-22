#ifndef _TEK_EDITOR_HPP
#define _TEK_EDITOR_HPP

#include "../core/tek_core.hpp"
#include "../drawing/tek_spritebatch.hpp"

bool tek_editor_init(u32 width, u32 height);
void tek_editor_destroy();
void tek_editor_update(float delta);
void tek_editor_render_3d();
void tek_editor_render_2d(TekSpritebatch* sb, TekFont* font);

bool tek_editor_save_map(const char* name);
bool tek_editor_load_map(const char* name);


#endif