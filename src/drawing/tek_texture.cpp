#include "tek_texture.hpp"

#include "../platform/tek_platform.hpp"

#include <stdio.h>
#include <stdlib.h>


void tek_tex_destroy(TekTexture *tex)
{
	GLCall(glDeleteTextures(1, &tex->id));
}

void tek_tex_bind(TekTexture *tex, u32 texture_unit)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, tex->id));
}

bool tek_tex_load(TekTexture *tex, const char *filename, TekTextureFilter filter)
{
	TekImage img;
	bool result = tek_img_load(&img, filename);
	if (!result)
	{
		return false;
	}

	result = tek_tex_create(tex, &img, filter);

	tek_img_destroy(&img);

	return result;
}

bool tek_tex_create(TekTexture *tex, TekImage *image, TekTextureFilter filter)
{
	tex->width = image->width;
	tex->height = image->height;

	GLenum filtering = GL_NEAREST;
	switch (filter)
	{
		case TEK_TEX_FILTER_NONE:
			filtering = GL_NEAREST;
			break;
		case TEK_TEX_FILTER_LINEAR:
			filtering = GL_LINEAR;
		default:
			break;
	}

	GLenum tex_format = GL_RGB;

	if (image->format == TEK_IMG_RGBA)
	{
		tex_format = GL_RGBA;
	}

	GLCall(glGenTextures(1, &tex->id));

	GLCall(glBindTexture(GL_TEXTURE_2D, tex->id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, tex_format, image->width, image->height, 0, tex_format,
	                    GL_UNSIGNED_BYTE, (GLvoid *) (image->pixels)));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	return true;
}

