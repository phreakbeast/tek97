#include "tek_texturebrowser.hpp"

static int g_selected_tex = 0;

void tek_texbrowser_handle_key(Key key, TekAssets* assets)
{
	if (key == KEY_LEFT)
	{
		g_selected_tex--;
		if (g_selected_tex < 0)
			g_selected_tex = 0;
	}
	else if (key == KEY_RIGHT)
	{
		g_selected_tex++;
		if (g_selected_tex >= assets->num_materials)
			g_selected_tex = assets->num_materials - 1;
	}
	else if (key == KEY_UP)
	{

	}
	else if (key == KEY_DOWN)
	{

	}
	else if (key == KEY_PAGE_UP)
	{

	}
	else if (key == KEY_PAGE_DOWN)
	{

	}
}

void tek_texbrowser_render(TekSpritebatch* sb, TekAssets* assets, TekFont* font)
{
	tek_sb_render_rect(sb, tek_rect_create(20, 20, 600, 320), tek_color_create(100, 100, 100));
	TekRect uv = { 0, 0, 1, 1 };
	;
	for (int i = 0; i < assets->num_materials; ++i)
	{
		if (g_selected_tex == i)
		{
			tek_sb_render_rect(sb, tek_rect_create(24 + 55 * i, 24, 52, 60), tek_color_create(50, 50, 50));
		}
		TekRect dest = { 25 + 55 * i, 25,50,50 };
		if (assets->materials[i].material.has_diffuse_map)
		{
			tek_sb_render_sprite(sb, dest, uv, assets->materials[i].material.diffuse_map.id);
		}
		else
		{
			tek_sb_render_rect(sb, dest, assets->materials[i].material.diffuse_color);
		}
		float x = dest.x;
		float y = dest.y + 50;
		tek_sb_render_text(sb, assets->materials[i].name, font, x, y, tek_color_white(), 50);
	}
}

int tek_texbrowser_get_selected_mat()
{
	return g_selected_tex;
}