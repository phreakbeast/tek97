#ifndef _TEK_MESHBROWSER_HPP
#define _TEK_MESHBROWSER_HPP

#include "../core/tek_core.hpp"
#include "tek_assets.hpp"
#include "../drawing/tek_spritebatch.hpp"

void tek_meshbrowser_handle_key(Key key, TekAssets* assets);
void tek_meshbrowser_render(TekSpritebatch* sb, TekAssets* assets, TekFont* font);

int tek_meshbrowser_get_selected_mesh();

#endif
