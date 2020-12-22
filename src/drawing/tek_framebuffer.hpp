#ifndef _TEK_FRAMEBUFFER_HPP
#define _TEK_FRAMEBUFFER_HPP

#include "../core/tek_core.hpp"
#include "tek_rect.hpp"
#include "../math/tek_math.hpp"
#include "tek_texture.hpp"
#include "tek_font.hpp"

class TekFramebuffer
{
public:
	TekFramebuffer(u32 width, u32 height);

	~TekFramebuffer();

	void bind_reading(u32 texture_unit);

	void bind_writing();

	static void unbind();

	const u32 get_width() const {return width;}
	const u32 get_height() const {return height;}
	const u32 get_tex_id() const {return tex_id;}
private:
	u32 fbo;
	u32 tex_id;
	u32 depth_id;

	u32 width;
	u32 height;
};

#endif
