#ifndef _TEK_DRAWING_HPP
#define _TEK_DRAWING_HPP

#include "../core/tek_core.hpp"
#include "tek_font.hpp"
#include "../math/tek_math.hpp"
#include "tek_framebuffer.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"
#include "tek_shader.hpp"
#include "tek_spritebatch.hpp"

namespace tek
{
	struct TekRenderStats
	{
		float delta;
		u32 fps;
		u32 ups;
		u32 num_sprites;
		u32 num_drawcalls;

		TekRenderStats()
		{
			delta = 0;
			fps = 0;
			ups = 0;
			num_sprites = 0;
			num_drawcalls = 0;
		}
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool init(u32 width, u32 height);

		void start_frame();

		void end_frame();

		void bind_framebuffer(TekFramebuffer *buffer, TekColor color);

		void unbind_framebuffer();

		void viewport(u32 width, u32 height);

		void disable_depth_test();

		void enable_depth_test();

		void clear(TekColor color);

		TekRenderStats* get_stats();

		TekSpritebatch* get_sb();

		void resize(u32 width, u32 height);

		void render_sprite(TekRect dest, TekRect src, u32 texture_id, Mat4 *ortho);
	private:
		TekSpritebatch* sprite_batch;

		TekRenderStats stats;
		u32 drawcalls = 0;

		TekShader sprite_shader;

		u32 width;
		u32 height;
	};
}

#endif
