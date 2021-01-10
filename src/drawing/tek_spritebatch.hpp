#ifndef _TEK_SPRITEBATCH_HPP
#define _TEK_SPRITEBATCH_HPP

#include "tek_shader.hpp"
#include "tek_font.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"

namespace tek
{

#define SB_MAX_TEXTURES 8

	class TekSpritebatch
	{
	public:
		struct TekSbVertexData
		{
			Vec3 position;
			Vec2 uv;
			float tex_id;
			u32 color;
		};

		u32 num_sprites;
		u32 drawcalls;

		TekSpritebatch();
		~TekSpritebatch();

		bool init(u32 width, u32 height);

		void begin();

		void end();

		void flush();

		void resize(u32 width, u32 height);

		void reset_stats();

		void render_rect(TekRect rect, TekColor color);

		void render_sprite(TekRect dest, TekRect src, u32 texture_id);

		void render_polygon(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, TekColor color);

		void render_text(const char *text, TekFont *font, int x, int y, TekColor color, float max_width);

		void render_circle(Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments, TekColor color);

		void render_line(Vec2 p0, Vec2 p1, float width, TekColor color);

	private:

		TekSbVertexData *buffer;

		u32 texture_slots[SB_MAX_TEXTURES];
		u32 num_slots;
		TekShader shader;
		Mat4 ortho;
		u32 vao;
		u32 vbo;
		u32 ibo;
		u32 index_count;
	};
}
#endif
