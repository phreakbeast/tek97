#ifndef _TEK_SPRITEBATCH_HPP
#define _TEK_SPRITEBATCH_HPP

#include "tek_shader.hpp"
#include "tek_font.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"

#define SB_MAX_TEXTURES 8

typedef struct
{
    Vec3 position;
    Vec2 uv;
    float tex_id;
    u32 color;
}TekSbVertexData;

typedef struct
{
    u32 num_sprites;
    u32 drawcalls;

    TekSbVertexData *buffer;

    u32 texture_slots[SB_MAX_TEXTURES];
    u32 num_slots;
    Shader* shader;
    Mat4 ortho;
    u32 vao;
    u32 vbo;
    u32 ibo;
    u32 index_count;
}TekSpritebatch;

bool tek_sb_init(TekSpritebatch* sb, u32 width, u32 height);

void tek_sb_destroy(TekSpritebatch* sb);

void tek_sb_begin(TekSpritebatch* sb);

void tek_sb_end(TekSpritebatch* sb);

void tek_sb_flush(TekSpritebatch* sb);

void tek_sb_resize(TekSpritebatch* sb, u32 width, u32 height);

void tek_sb_reset_stats(TekSpritebatch* sb);

void tek_sb_render_rect(TekSpritebatch* sb, TekRect rect, TekColor color);

void tek_sb_render_sprite(TekSpritebatch* sb,TekRect dest, TekRect src, u32 texture_id);

void tek_sb_render_polygon(TekSpritebatch* sb, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, TekColor color);

void tek_sb_render_text(TekSpritebatch* sb, const char *text, TekFont *font, int x, int y, TekColor color, float max_width);

void tek_sb_render_circle(TekSpritebatch* sb, Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments, TekColor color);

void tek_sb_render_line(TekSpritebatch* sb, Vec2 p0, Vec2 p1, float width, TekColor color);

#endif
