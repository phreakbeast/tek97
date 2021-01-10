#include "tek_spritebatch.hpp"

#include "../platform/tek_platform.hpp"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

namespace tek
{

#define SB_MAX_SPRITES    60000
#define SB_VERTEX_SIZE    sizeof(TekSbVertexData)
#define SB_SPRITE_SIZE    SB_VERTEX_SIZE * 4
#define SB_BUFFER_SIZE    SB_SPRITE_SIZE * SB_MAX_SPRITES
#define SB_INDICES_SIZE   SB_MAX_SPRITES * 6

	TekSpritebatch::TekSpritebatch()
	{
		num_sprites = 0;
		drawcalls = 0;

		buffer = nullptr;

		num_slots = 0;
		for (u32 i = 0; i < SB_MAX_TEXTURES; ++i)
		{
			texture_slots[i] = 0;
		}

		vao = 0;
		vbo = 0;
		ibo = 0;
		index_count = 0;
	}

	TekSpritebatch::~TekSpritebatch()
	{
		GLCall(glDeleteBuffers(1, &vbo));
		GLCall(glDeleteBuffers(1, &ibo));
		GLCall(glDeleteVertexArrays(1, &vao));
		tek_shader_destroy(&shader);
	}

	bool TekSpritebatch::init(u32 width, u32 height)
	{
		ortho = mat4_ortho(0, (float) width, 0, (float) height, 1, -1);
		ortho = mat4_transposed(&ortho);

		//Shader
		bool result = tek_shader_load(&shader, "sprite.glsl");
		if (!result)
			return false;

		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		GLCall(glGenBuffers(1, &vbo));
		GLCall(glGenBuffers(1, &ibo));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLCall(glBufferData(GL_ARRAY_BUFFER, SB_BUFFER_SIZE, NULL, GL_STATIC_DRAW));

		int loc_pos = tek_shader_get_attrib_loc(&shader, "a_position");
		int loc_uv = tek_shader_get_attrib_loc(&shader, "a_uv");
		int loc_tid = tek_shader_get_attrib_loc(&shader, "a_tid");
		int loc_color = tek_shader_get_attrib_loc(&shader, "a_color");

		GLCall(glEnableVertexAttribArray(loc_pos));
		GLCall(glEnableVertexAttribArray(loc_uv));
		GLCall(glEnableVertexAttribArray(loc_tid));
		GLCall(glEnableVertexAttribArray(loc_color));

		GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
		                             (const GLvoid *) (offsetof(TekSbVertexData, position))));

		GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
		                             (const GLvoid *) (offsetof(TekSbVertexData, uv))));

		GLCall(glVertexAttribPointer(loc_tid, 1, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
		                             (const GLvoid *) (offsetof(TekSbVertexData, tex_id))));

		GLCall(glVertexAttribPointer(loc_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, SB_VERTEX_SIZE,
		                             (const GLvoid *) (offsetof(TekSbVertexData, color))));


		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


		GLuint *indices = (GLuint *) tek_malloc(sizeof(GLuint) * SB_INDICES_SIZE);
		int offset = 0;
		for (int i = 0; i < SB_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;

			offset += 4;
		}

		GLCall(glDisableVertexAttribArray(loc_pos));
		GLCall(glDisableVertexAttribArray(loc_uv));
		GLCall(glDisableVertexAttribArray(loc_tid));
		GLCall(glDisableVertexAttribArray(loc_color));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, SB_INDICES_SIZE * sizeof(GLuint), indices, GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		tek_free(indices);

		return true;
	}

	void TekSpritebatch::begin()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLCall(buffer = (TekSbVertexData *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	void TekSpritebatch::end()
	{
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
	}

	void TekSpritebatch::flush()
	{
		tek_shader_bind(&shader);

		tek_shader_uniform_mat4(&shader, "u_mvp", &ortho, 1, false);


		tek_shader_uniform_int(&shader, "u_texture_0", 0);
		tek_shader_uniform_int(&shader, "u_texture_1", 1);
		tek_shader_uniform_int(&shader, "u_texture_2", 2);
		tek_shader_uniform_int(&shader, "u_texture_3", 3);
		tek_shader_uniform_int(&shader, "u_texture_4", 4);
		tek_shader_uniform_int(&shader, "u_texture_5", 5);
		tek_shader_uniform_int(&shader, "u_texture_6", 6);
		tek_shader_uniform_int(&shader, "u_texture_7", 7);

		for (u32 i = 0; i < num_slots; ++i)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, texture_slots[i]));
		}

		GLCall(glBindVertexArray(vao));

		int loc_pos = tek_shader_get_attrib_loc(&shader, "a_position");
		int loc_uv = tek_shader_get_attrib_loc(&shader, "a_uv");
		int loc_tid = tek_shader_get_attrib_loc(&shader, "a_tid");
		int loc_color = tek_shader_get_attrib_loc(&shader, "a_color");

		GLCall(glEnableVertexAttribArray(loc_pos));
		GLCall(glEnableVertexAttribArray(loc_uv));
		GLCall(glEnableVertexAttribArray(loc_tid));
		GLCall(glEnableVertexAttribArray(loc_color));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));

		GLCall(glDisableVertexAttribArray(loc_pos));
		GLCall(glDisableVertexAttribArray(loc_uv));
		GLCall(glDisableVertexAttribArray(loc_tid));
		GLCall(glDisableVertexAttribArray(loc_color));

		//TODO: clear texture slots
		drawcalls++;
		index_count = 0;
		num_slots = 0;
		num_sprites = 0;
	}

	void TekSpritebatch::resize(u32 width, u32 height)
	{
		ortho = mat4_ortho(0, (float) width, 0, (float) height, 1, -1);
		ortho = mat4_transposed(&ortho);
	}

	void TekSpritebatch::reset_stats()
	{
		drawcalls = 0;
		num_sprites = 0;
	}

	void TekSpritebatch::render_rect(TekRect rect, TekColor color)
	{
		if (num_sprites >= SB_MAX_SPRITES)
		{
			end();
			flush();
			begin();
		}

		const Vec3 position = {rect.x, rect.y, 0};
		const Vec2 size = {rect.w, rect.h};
		const u32 col = color.to_int();
		const Vec2 uv = {0, 0};
		const float tid = 0;

		{
			buffer->position = position;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x, position.y + size.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x + size.x, position.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		num_sprites++;
		index_count += 6;
	}

	void TekSpritebatch::render_sprite(TekRect dest, TekRect src, u32 texture_id)
	{
		if (num_sprites >= SB_MAX_SPRITES)
		{
			end();
			flush();
			begin();
		}

		const Vec3 position = {dest.x, dest.y, 0};
		const Vec2 size = {dest.w, dest.h};
		const u32 col = TekColor::floats_to_int(1, 1, 1, 1);
		const GLuint tid = texture_id;

		Vec2 uv[4];
		uv[0].x = src.x;
		uv[0].y = src.h;
		uv[1].x = src.x;
		uv[1].y = src.y;
		uv[2].x = src.w;
		uv[2].y = src.y;
		uv[3].x = src.w;
		uv[3].y = src.h;

		float ts = 0.0f;
		if (tid > 0)
		{
			u32 found = 0;
			for (u32 i = 0; i < num_slots; i++)
			{
				if (texture_slots[i] == tid)
				{
					ts = (float) (i + 1);
					found = 1;
					break;
				}
			}

			if (!found)
			{
				if (num_slots >= SB_MAX_TEXTURES)
				{
					begin();
					end();
					flush();
				}
				else
				{
					texture_slots[num_slots++] = tid;
					ts = (float) (num_slots);
				}
			}
		}


		{
			buffer->position = position;
			buffer->uv = uv[0];
			buffer->tex_id = ts;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x, position.y + size.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv[1];
			buffer->tex_id = ts;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv[2];
			buffer->tex_id = ts;
			buffer->color = col;
			buffer++;
		}

		{
			Vec3 new_pos = {position.x + size.x, position.y, position.z};
			buffer->position = new_pos;
			buffer->uv = uv[3];
			buffer->tex_id = ts;
			buffer->color = col;
			buffer++;
		}

		num_sprites++;
		index_count += 6;
	}

	void TekSpritebatch::render_polygon(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, TekColor color)
	{
		if (num_sprites >= SB_MAX_SPRITES)
		{
			end();
			flush();
			begin();
		}

		const Vec3 v0 = {p0.x, p0.y, 0};
		const Vec3 v1 = {p1.x, p1.y, 0};
		const Vec3 v2 = {p2.x, p2.y, 0};
		const Vec3 v3 = {p3.x, p3.y, 0};

		const u32 col = color.to_int();
		const Vec2 uv = {0, 0};
		const float tid = 0;

		{
			buffer->position = v0;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			buffer->position = v1;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			buffer->position = v2;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		{
			buffer->position = v3;
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;
		}

		num_sprites++;
		index_count += 6;
	}

	void TekSpritebatch::render_text(const char *text, TekFont *font, int x, int y, TekColor color, float max_width)
	{
		if (num_sprites >= SB_MAX_SPRITES)
		{
			end();
			flush();
			begin();
		}

		size_t len = strlen(text);
		int pos_x = x;
		int pos_y = y;

		if (max_width != 0)
		{
			//check if the text is too long
			float width = len * font->width;
			if (width > max_width)
			{
				float diff = max_width / font->width;
				len = (size_t) diff;
			}
		}

		for (u32 i = 0; i < len; ++i)
		{
			char c = text[i];

			if (c == '\n')
			{
				pos_y += font->height;
				pos_x = x;
				continue;
			}

			const TekFontLetter *letter = font->get_letter(c);
			if (letter == NULL)
			{
				letter = font->get_letter('?');
			}

			const Vec3 position = {(float) pos_x, (float) pos_y, 0};
			const Vec2 size = {(float) letter->width, (float) letter->width};
			const unsigned int col = color.to_int();
			Vec2 uv[4];

			uv[0].x = letter->uv_l;
			uv[0].y = letter->uv_t;
			uv[1].x = letter->uv_l;
			uv[1].y = letter->uv_b;
			uv[2].x = letter->uv_r;
			uv[2].y = letter->uv_b;
			uv[3].x = letter->uv_r;
			uv[3].y = letter->uv_t;

			const GLuint tid = font->texture.id;

			float ts = 0.0f;
			if (tid > 0)
			{
				u32 found = 0;
				for (u32 i = 0; i < num_slots; i++)
				{
					if (texture_slots[i] == tid)
					{
						ts = (float) (i + 1);
						found = 1;
						break;
					}
				}

				if (!found)
				{
					if (num_slots >= SB_MAX_TEXTURES)
					{
						begin();
						end();
						flush();
					}

					texture_slots[num_slots++] = tid;
					ts = (float) (num_slots);
				}
			}

			{
				buffer->position = position;
				buffer->uv = uv[0];
				buffer->tex_id = ts;
				buffer->color = col;
				buffer++;
			}

			{
				Vec3 new_pos = {position.x, position.y + size.y, position.z};
				buffer->position = new_pos;
				buffer->uv = uv[1];
				buffer->tex_id = ts;
				buffer->color = col;
				buffer++;
			}

			{
				Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
				buffer->position = new_pos;
				buffer->uv = uv[2];
				buffer->tex_id = ts;
				buffer->color = col;
				buffer++;
			}

			{
				Vec3 new_pos = {position.x + size.x, position.y, position.z};
				buffer->position = new_pos;
				buffer->uv = uv[3];
				buffer->tex_id = ts;
				buffer->color = col;
				buffer++;
			}

			index_count += 6;
			pos_x += letter->width;
			num_sprites++;
		}
	}

	void TekSpritebatch::render_circle(Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments, TekColor color)
	{
		//TODO: check if buffers is big enough

		Vec3 *points = (Vec3 *) tek_malloc(sizeof(Vec3) * num_segments + 2);

		float step = 0;
		if (end_angle > start_angle)
		{
			step = (end_angle - start_angle) / num_segments;
		}
		else
		{
			step = ((360 - start_angle) + end_angle) / num_segments;
		}

		for (u32 i = 0; i < num_segments + 1; ++i)
		{
			float angle = start_angle + i * step;
			float x, y;
			math_get_circle_coord(radius, pos.x, pos.y, angle, &x, &y);
			points[i].x = x;
			points[i].y = y;
			points[i].z = 0;
		}

		points[num_segments + 1].x = pos.x;
		points[num_segments + 1].y = pos.y;
		points[num_segments + 1].z = 0;

		const u32 col = color.to_int();
		const Vec2 uv = {0, 0};
		const float tid = 0;

		for (u32 i = 0; i < num_segments + 1; ++i)
		{
			buffer->position = points[i];
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;

			buffer->position = points[num_segments + 1];
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;

			buffer->position = points[num_segments + 1];
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;

			buffer->position = points[i + 1];
			buffer->uv = uv;
			buffer->tex_id = tid;
			buffer->color = col;
			buffer++;

			index_count += 6;
			num_sprites++;
		}

		tek_free(points);
	}

	void TekSpritebatch::render_line(Vec2 p0, Vec2 p1, float width, TekColor color)
	{
		if (num_sprites >= SB_MAX_SPRITES)
		{
			end();
			flush();
			begin();
		}

		//TODO: simplify with vec2 calculation
		Vec3 new_pos[4];

		Vec2 vec_l = p0;
		Vec2 vec_r = p1;
		if (p0.x > p1.x)
		{
			vec_l = p1;
			vec_r = p0;
		}

		Vec2 vec = vec2_sub(vec_r, vec_l);
		Vec2 vec_p = vec2_create(-vec.y, vec.x);
		//Vec2 vec_pp = vec2_add(vec_p, vec_l);

		Vec2 vec_p2 = vec2_create(vec.y, -vec.x);
		//Vec2 vec_pp2 = vec2_add(vec_p2, vec_l);

		//Vec2 vec_ppp = vec_pp + vec;
		//Vec2 vec_ppp2 = vec_pp2 + vec;

		Vec2 vec_n1 = vec2_normalized(vec_p);
		Vec2 vec_n2 = vec2_normalized(vec_p2);

		Vec2 v0 = vec2_add(vec_l, vec2_mul(vec_n2, width));
		Vec2 v1 = vec2_add(vec_l, vec2_mul(vec_n1, width));
		Vec2 v2 = vec2_add(vec_r, vec2_mul(vec_n1, width));
		Vec2 v3 = vec2_add(vec_r, vec2_mul(vec_n2, width));

		new_pos[0].x = v0.x;
		new_pos[0].y = v0.y;
		new_pos[0].z = 0;

		new_pos[1].x = v1.x;
		new_pos[1].y = v1.y;
		new_pos[1].z = 0;

		new_pos[2].x = v2.x;
		new_pos[2].y = v2.y;
		new_pos[2].z = 0;

		new_pos[3].x = v3.x;
		new_pos[3].y = v3.y;
		new_pos[3].z = 0;

		const u32 col = color.to_int();
		const Vec2 uv = {0, 0};
		const float tid = 0;

		buffer->position = new_pos[0];
		buffer->uv = uv;
		buffer->tex_id = tid;
		buffer->color = col;
		buffer++;

		buffer->position = new_pos[1];
		buffer->uv = uv;
		buffer->tex_id = tid;
		buffer->color = col;
		buffer++;

		buffer->position = new_pos[2];
		buffer->uv = uv;
		buffer->tex_id = tid;
		buffer->color = col;
		buffer++;

		buffer->position = new_pos[3];
		buffer->uv = uv;
		buffer->tex_id = tid;
		buffer->color = col;
		buffer++;

		index_count += 6;
		num_sprites++;
	}
}
