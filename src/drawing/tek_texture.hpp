#ifndef _TEK_TEXTURE_HPP
#define _TEK_TEXTURE_HPP

#include "../core/tek_core.hpp"
#include "tek_color.hpp"
#include "tek_image.hpp"

namespace tek
{

	typedef enum
	{
		TEK_TEX_FILTER_NONE,
		TEK_TEX_FILTER_LINEAR
	} TekTextureFilter;

	typedef struct
	{
		u32 id;
		u32 width;
		u32 height;
	} TekTexture;

	void tek_tex_destroy(TekTexture *tex);

	void tek_tex_bind(TekTexture *tex, u32 texture_unit);

	bool tek_tex_load(TekTexture *tex, const char *filename, TekTextureFilter filter);

	bool tek_tex_create(TekTexture *tex, TekImage *image, TekTextureFilter filter);
}
#endif
