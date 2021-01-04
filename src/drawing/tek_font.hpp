#ifndef _TEK_FONT_HPP
#define _TEK_FONT_HPP

#include "../core/tek_core.hpp"
#include "tek_texture.hpp"

struct TekFontLetter
{
	char character;
	float uv_l;
	float uv_r;
	float uv_t;
	float uv_b;
	int width;
	int x_offset;
	int y_offset;
};

struct TekKerningTable
{
	int val[512][512];
};

struct TekFont
{
	TekTexture texture;
	TekKerningTable kernings;
	TekFontLetter letters[512];
	int num_letters;
	int width;
	int height;
};


void tek_font_destroy(TekFont* font);

const TekFontLetter* tek_font_get_letter(TekFont* font, char character);

const u32 tek_font_text_length(TekFont* font, const char* text);

const u32 tek_font_text_height(TekFont* font, const char* text);

bool tek_font_load(TekFont* font, const char* filename);

#endif
