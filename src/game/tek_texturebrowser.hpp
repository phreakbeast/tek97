#ifndef _TEK_TEXTUREBROWSER_HPP
#define _TEK_TEXTUREBROWSER_HPP

#include "../core/tek_core.hpp"
#include "tek_assets.hpp"
#include "../drawing/tek_spritebatch.hpp"

void tek_texbrowser_handle_key(Key key, TekAssets* assets);
void tek_texbrowser_render(TekSpritebatch* sb, TekAssets* assets, TekFont* font);

int tek_texbrowser_get_selected_mat();

#endif
