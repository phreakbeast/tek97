#ifndef _TEK_FONT_HPP
#define _TEK_FONT_HPP

#include "../core/tek_core.hpp"
#include "tek_texture.hpp"

namespace tek
{
    typedef struct
    {
	char character;
	float uv_l;
	float uv_r;
	float uv_t;
	float uv_b;
	int width;
	int x_offset;
	int y_offset;
    }TekFontLetter;

    typedef struct
    {
	int val[512][512];
    }TekKerningTable;

    struct TekFont
    {
	TekTexture texture;
	TekKerningTable kernings;
	TekFontLetter letters[512];
	int num_letters;
	int width;
	int height;

	TekFont();
	~TekFont();
	
	const TekFontLetter* get_letter(char character) const;

	const u32 text_length(const char* text) const;

	const u32 text_height(const char* text) const;

	static TekFont* load(const char* filename);
    };
}
#endif
