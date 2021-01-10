#ifndef _TEK_FRAMEBUFFER_HPP
#define _TEK_FRAMEBUFFER_HPP

#include "../core/tek_core.hpp"
#include "tek_rect.hpp"
#include "../math/tek_math.hpp"

namespace tek
{
	struct TekFramebuffer
	{
		u32 fbo;
		u32 tex_id;
		u32 depth_id;

		u32 width;
		u32 height;

		TekFramebuffer(u32 width, u32 height);
		~TekFramebuffer();

		void bind_reading(u32 texture_unit);

		void bind_writing();

		static void unbind();
	};
}
#endif
