#include "tek_font.hpp"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

namespace tek
{
	TekFont::TekFont()
	{
		num_letters = 0;
	}

	TekFont::~TekFont()
	{
		tek_tex_destroy(&texture);
	}

	const TekFontLetter *TekFont::get_letter(char character) const
	{
		//TODO: replace with something faster
		for (int i = 0; i < num_letters; ++i)
		{
			if (letters[i].character == character)
			{
				return &letters[i];
			}
		}
		return nullptr;
	}

	const u32 TekFont::text_length(const char *text) const
	{
		u32 res = 0;
		u32 len = (u32) strlen(text);
		for (u32 i = 0; i < len; ++i)
		{
			char c = text[i];
			const TekFontLetter *letter = get_letter(c);
			if (letter == NULL)
			{
				letter = get_letter('?');
			}
			res += letter->width;
		}
		return res;
	}

	const u32 TekFont::text_height(const char *text) const
	{
		//TODO: implement
		return height;
	}


	TekFont *TekFont::load(const char *filename)
	{
		printf("loading font %s\n", filename);
		FILE *fp = fopen(filename, "rb");
		if (!fp)
		{
			printf("Error: cannot open file %s\n", filename);
			return nullptr;
		}

		TekFont *font = new TekFont();

		char line[1024];
		while (fgets(line, 1024, fp))
		{
			if (line[0] == '\n')
			{ continue; }
			if (line[0] == '\r' && line[1] == '\n')
			{ continue; }
			if (line[0] == '\0')
			{ continue; }

			if (line[0] == '#')
			{ continue; }

			if (line[0] == 'h' && line[1] == 'd' && line[2] == 'r')
			{
				char tex_name[256];
				int width;
				int height;

				int res = sscanf(line, "hdr %s %d %d", tex_name, &width, &height);
				if (res < 3)
				{
					printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
				}

				bool result = tek_tex_load(&font->texture, tex_name, TEK_TEX_FILTER_LINEAR);
				if (!result)
				{
					fclose(fp);
					delete font;
					return nullptr;
				}

				font->width = width;
				font->height = height;
			}
			else if (line[0] == 'l' && line[1] == 't' && line[2] == 'r')
			{
				char character;
				int uv_x;
				int uv_y;
				int uv_w;
				int uv_h;
				int x_offset;
				int y_offset;

				int res = sscanf(line, "ltr %c %d %d %d %d %d %d", &character, &uv_x, &uv_y, &uv_w, &uv_h, &x_offset,
				                 &y_offset);
				if (res < 7)
				{
					printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
				}

				TekFontLetter letter;
				letter.character = character;
				letter.width = uv_w;
				letter.x_offset = x_offset;
				letter.y_offset = y_offset;

				int tex_w = font->texture.width;
				int tex_h = font->texture.height;
				assert(tex_w != 0 && tex_h != 0);

				letter.uv_l = uv_x / (float) tex_w;
				letter.uv_r = letter.uv_l + uv_w / (float) tex_w;
				letter.uv_t = (tex_h - uv_y) / (float) tex_h;
				letter.uv_b = letter.uv_t - uv_h / (float) tex_h;


				font->letters[font->num_letters++] = letter;
			}
			else if (line[0] == 'k' && line[1] == 'r' && line[2] == 'n')
			{
				char letter1;
				char letter2;
				int val;

				int res = sscanf(line, "krn %c %c %d", &letter1, &letter2, &val);
				if (res < 3)
				{
					printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
				}

				font->kernings.val[(int) letter1][(int) letter2] = val;
			}
			else
			{
				printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
			}
		}

		fclose(fp);

		//insert space
		TekFontLetter letter;
		letter.character = ' ';
		letter.width = font->width;
		letter.x_offset = 0;
		letter.y_offset = 0;

		letter.uv_l = 0;
		letter.uv_r = 0;
		letter.uv_t = 0;
		letter.uv_b = 0;
		assert(font->num_letters < 512);
		font->letters[font->num_letters++] = letter;

		return font;
	}
}
