#include "tek_meshbrowser.hpp"

static int g_selected_mesh = 0;

void tek_meshbrowser_handle_key(Key key, TekAssets* assets)
{
	if (key == KEY_LEFT)
	{
		
	}
	else if (key == KEY_RIGHT)
	{
		
	}
	else if (key == KEY_UP)
	{
		g_selected_mesh--;
		if (g_selected_mesh < 0)
			g_selected_mesh = 0;
	}
	else if (key == KEY_DOWN)
	{
		g_selected_mesh++;
		if (g_selected_mesh >= assets->num_meshes)
			g_selected_mesh = assets->num_meshes - 1;
	}
	else if (key == KEY_PAGE_UP)
	{

	}
	else if (key == KEY_PAGE_DOWN)
	{

	}
}

void tek_meshbrowser_render(TekSpritebatch* sb, TekAssets* assets, TekFont* font)
{
	tek_sb_render_rect(sb, tek_rect_create(20, 20, 600, 320), tek_color_create(100, 100, 100));
	TekRect uv = { 0, 0, 1, 1 };
	;
	for (int i = 0; i < assets->num_meshes; ++i)
	{
		TekRect dest = { 25, 25 + 10*i,200,10 };
		if (g_selected_mesh == i)
		{
			tek_sb_render_rect(sb, dest, tek_color_create(50, 50, 50));
		}
		
		
		float x = dest.x;
		float y = dest.y;
		tek_sb_render_text(sb, assets->meshes[i].name, font, x, y, tek_color_white(), 200);
	}
}

int tek_meshbrowser_get_selected_mesh()
{
	return g_selected_mesh;
}