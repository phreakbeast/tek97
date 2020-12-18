#include "tek_spritebatch.hpp"

#include "../platform/tek_platform.hpp"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define SB_MAX_SPRITES    60000
#define SB_VERTEX_SIZE    sizeof(TekSbVertexData)
#define SB_SPRITE_SIZE    SB_VERTEX_SIZE * 4
#define SB_BUFFER_SIZE    SB_SPRITE_SIZE * SB_MAX_SPRITES
#define SB_INDICES_SIZE   SB_MAX_SPRITES * 6


void tek_sb_destroy(TekSpritebatch *sb)
{
    GLCall(glDeleteBuffers(1, &sb->vbo));
    GLCall(glDeleteBuffers(1, &sb->ibo));
    GLCall(glDeleteVertexArrays(1, &sb->vao));
    tek_shader_destroy(&sb->shader);
}

bool tek_sb_init(TekSpritebatch *sb, u32 width, u32 height)
{
    sb->num_sprites = 0;
    sb->drawcalls = 0;

    sb->buffer = NULL;

    sb->num_slots = 0;
    for (u32 i = 0; i < SB_MAX_TEXTURES; ++i)
    {
        sb->texture_slots[i] = 0;
    }

    sb->vao = 0;
    sb->vbo = 0;
    sb->ibo = 0;
    sb->index_count = 0;

    sb->ortho = mat4_ortho(0, (float) width, 0, (float) height, 1, -1);
    sb->ortho = mat4_transposed(&sb->ortho);

    //Shader
    bool result = tek_shader_load(&sb->shader, "sprite.glsl");
    if (!result)
        return false;

    GLCall(glGenVertexArrays(1, &sb->vao));
    GLCall(glBindVertexArray(sb->vao));

    GLCall(glGenBuffers(1, &sb->vbo));
    GLCall(glGenBuffers(1, &sb->ibo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, sb->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, SB_BUFFER_SIZE, NULL, GL_STATIC_DRAW));

    int loc_pos = tek_shader_get_attrib_loc(&sb->shader, "a_position");
    int loc_uv = tek_shader_get_attrib_loc(&sb->shader, "a_uv");
    int loc_tid = tek_shader_get_attrib_loc(&sb->shader, "a_tid");
    int loc_color = tek_shader_get_attrib_loc(&sb->shader, "a_color");

    GLCall(glEnableVertexAttribArray(loc_pos));
    GLCall(glEnableVertexAttribArray(loc_uv));
    GLCall(glEnableVertexAttribArray(loc_tid));
    GLCall(glEnableVertexAttribArray(loc_color));

    GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
                                 (const GLvoid *) (offsetof(TekSbVertexData, position))));

    GLCall(glVertexAttribPointer(loc_uv , 2, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
                                 (const GLvoid *) (offsetof(TekSbVertexData, uv))));

    GLCall(glVertexAttribPointer(loc_tid, 1, GL_FLOAT, GL_FALSE, SB_VERTEX_SIZE,
                                 (const GLvoid *) (offsetof(TekSbVertexData, tex_id))));

    GLCall(glVertexAttribPointer(loc_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, SB_VERTEX_SIZE,
                                 (const GLvoid *) (offsetof(TekSbVertexData, color))));


    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


    GLuint *indices = (GLuint*)tek_malloc(sizeof(GLuint) * SB_INDICES_SIZE);
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

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sb->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, SB_INDICES_SIZE * sizeof(GLuint), indices, GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    tek_free(indices);

    return true;
}

void tek_sb_begin(TekSpritebatch *sb)
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, sb->vbo));
    GLCall(sb->buffer = (TekSbVertexData *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
}

void tek_sb_end(TekSpritebatch *sb)
{
    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}

void tek_sb_flush(TekSpritebatch *sb)
{
    tek_shader_bind(&sb->shader);

    tek_shader_uniform_mat4(&sb->shader, "u_mvp", &sb->ortho, 1);


    tek_shader_uniform_int(&sb->shader, "u_texture_0", 0);
    tek_shader_uniform_int(&sb->shader, "u_texture_1", 1);
    tek_shader_uniform_int(&sb->shader, "u_texture_2", 2);
    tek_shader_uniform_int(&sb->shader, "u_texture_3", 3);
    tek_shader_uniform_int(&sb->shader, "u_texture_4", 4);
    tek_shader_uniform_int(&sb->shader, "u_texture_5", 5);
    tek_shader_uniform_int(&sb->shader, "u_texture_6", 6);
    tek_shader_uniform_int(&sb->shader, "u_texture_7", 7);

    for (u32 i = 0; i < sb->num_slots; ++i)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));
        GLCall(glBindTexture(GL_TEXTURE_2D, sb->texture_slots[i]));
    }

    GLCall(glBindVertexArray(sb->vao));

    int loc_pos = tek_shader_get_attrib_loc(&sb->shader, "a_position");
    int loc_uv = tek_shader_get_attrib_loc(&sb->shader, "a_uv");
    int loc_tid = tek_shader_get_attrib_loc(&sb->shader, "a_tid");
    int loc_color = tek_shader_get_attrib_loc(&sb->shader, "a_color");

    GLCall(glEnableVertexAttribArray(loc_pos));
    GLCall(glEnableVertexAttribArray(loc_uv));
    GLCall(glEnableVertexAttribArray(loc_tid));
    GLCall(glEnableVertexAttribArray(loc_color));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sb->ibo));
    GLCall(glDrawElements(GL_TRIANGLES, sb->index_count, GL_UNSIGNED_INT, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    GLCall(glDisableVertexAttribArray(loc_pos));
    GLCall(glDisableVertexAttribArray(loc_uv));
    GLCall(glDisableVertexAttribArray(loc_tid));
    GLCall(glDisableVertexAttribArray(loc_color));

    //TODO: clear texture slots
    sb->drawcalls++;
    sb->index_count = 0;
    sb->num_slots = 0;
}

void tek_sb_resize(TekSpritebatch *sb, u32 width, u32 height)
{
    sb->ortho = mat4_ortho(0, (float) width, 0, (float) height, 1, -1);
    sb->ortho = mat4_transposed(&sb->ortho);
}

void tek_sb_reset_stats(TekSpritebatch *sb)
{
    sb->drawcalls = 0;
    sb->num_sprites = 0;
}

void tek_sb_render_rect(TekSpritebatch *sb, TekRect rect, TekColor color)
{
    const Vec3 position = {rect.x, rect.y, 0};
    const Vec2 size = {rect.w, rect.h};
    const u32 col = tek_color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;

    {
        sb->buffer->position = position;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x, position.y + size.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x + size.x, position.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    sb->num_sprites++;
    sb->index_count += 6;
}

void tek_sb_render_sprite(TekSpritebatch *sb, TekRect dest, TekRect src, u32 texture_id)
{
    const Vec3 position = {dest.x, dest.y, 0};
    const Vec2 size = {dest.w, dest.h};
    const u32 col = tek_color_floats_to_int(1, 1, 1, 1);
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
        for (u32 i = 0; i < sb->num_slots; i++)
        {
            if (sb->texture_slots[i] == tid)
            {
                ts = (float) (i + 1);
                found = 1;
                break;
            }
        }

        if (!found)
        {
            if (sb->num_slots >= SB_MAX_TEXTURES)
            {
                tek_sb_end(sb);
                tek_sb_flush(sb);
                tek_sb_begin(sb);
            }
            else
            {
                sb->texture_slots[sb->num_slots++] = tid;
                ts = (float) (sb->num_slots);
            }
        }
    }


    {
        sb->buffer->position = position;
        sb->buffer->uv = uv[0];
        sb->buffer->tex_id = ts;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x, position.y + size.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv[1];
        sb->buffer->tex_id = ts;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv[2];
        sb->buffer->tex_id = ts;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        Vec3 new_pos = {position.x + size.x, position.y, position.z};
        sb->buffer->position = new_pos;
        sb->buffer->uv = uv[3];
        sb->buffer->tex_id = ts;
        sb->buffer->color = col;
        sb->buffer++;
    }

    sb->num_sprites++;
    sb->index_count += 6;
}

void tek_sb_render_polygon(TekSpritebatch *sb, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, TekColor color)
{
    const Vec3 v0 = {p0.x, p0.y, 0};
    const Vec3 v1 = {p1.x, p1.y, 0};
    const Vec3 v2 = {p2.x, p2.y, 0};
    const Vec3 v3 = {p3.x, p3.y, 0};

    const u32 col = tek_color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;

    {
        sb->buffer->position = v0;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        sb->buffer->position = v1;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        sb->buffer->position = v2;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    {
        sb->buffer->position = v3;
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;
    }

    sb->num_sprites++;
    sb->index_count += 6;
}

void tek_sb_render_text(TekSpritebatch *sb, const char *text, TekFont *font, int x, int y, TekColor  color, float max_width)
{
    size_t len = strlen(text);
    int pos_x = x;
    int pos_y = y;

    if(max_width != 0)
    {
        //check if the text is too long
        float width = len * font->width;
        if(width > max_width)
        {
            float diff = max_width / font->width;
            len = (size_t)diff;
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

        const TekFontLetter *letter = tek_font_get_letter(font, c);
        if (letter == NULL)
        {
            letter = tek_font_get_letter(font, '?');
        }

        const Vec3 position = {(float) pos_x, (float) pos_y, 0};
        const Vec2 size = {(float) letter->width, (float) letter->width};
        const unsigned int col = tek_color_to_int(color);
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
            for (u32 i = 0; i < sb->num_slots; i++)
            {
                if (sb->texture_slots[i] == tid)
                {
                    ts = (float) (i + 1);
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                if (sb->num_slots >= SB_MAX_TEXTURES)
                {
                    tek_sb_end(sb);
                    tek_sb_flush(sb);
                    tek_sb_begin(sb);
                }

                sb->texture_slots[sb->num_slots++] = tid;
                ts = (float) (sb->num_slots);
            }
        }

        {
            sb->buffer->position = position;
            sb->buffer->uv = uv[0];
            sb->buffer->tex_id = ts;
            sb->buffer->color = col;
            sb->buffer++;
        }

        {
            Vec3 new_pos = {position.x, position.y + size.y, position.z};
            sb->buffer->position = new_pos;
            sb->buffer->uv = uv[1];
            sb->buffer->tex_id = ts;
            sb->buffer->color = col;
            sb->buffer++;
        }

        {
            Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
            sb->buffer->position = new_pos;
            sb->buffer->uv = uv[2];
            sb->buffer->tex_id = ts;
            sb->buffer->color = col;
            sb->buffer++;
        }

        {
            Vec3 new_pos = {position.x + size.x, position.y, position.z};
            sb->buffer->position = new_pos;
            sb->buffer->uv = uv[3];
            sb->buffer->tex_id = ts;
            sb->buffer->color = col;
            sb->buffer++;
        }

        sb->index_count += 6;
        pos_x += letter->width;
        sb->num_sprites++;
    }
}

void
tek_sb_render_circle(TekSpritebatch *sb, Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments,
                     TekColor color)
{
    Vec3 *points = (Vec3*)tek_malloc(sizeof(Vec3) * num_segments + 2);

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

    const u32 col = tek_color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;

    for (u32 i = 0; i < num_segments + 1; ++i)
    {
        sb->buffer->position = points[i];
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;

        sb->buffer->position = points[num_segments + 1];
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;

        sb->buffer->position = points[num_segments + 1];
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;

        sb->buffer->position = points[i + 1];
        sb->buffer->uv = uv;
        sb->buffer->tex_id = tid;
        sb->buffer->color = col;
        sb->buffer++;

        sb->index_count += 6;
        sb->num_sprites++;
    }

    tek_free(points);
}

void tek_sb_render_line(TekSpritebatch *sb, Vec2 p0, Vec2 p1, float width, TekColor color)
{

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
    Vec2 vec_pp = vec2_add(vec_p, vec_l);

    Vec2 vec_p2 = vec2_create(vec.y, -vec.x);
    Vec2 vec_pp2 = vec2_add(vec_p2, vec_l);

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

    const u32 col = tek_color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;

    sb->buffer->position = new_pos[0];
    sb->buffer->uv = uv;
    sb->buffer->tex_id = tid;
    sb->buffer->color = col;
    sb->buffer++;

    sb->buffer->position = new_pos[1];
    sb->buffer->uv = uv;
    sb->buffer->tex_id = tid;
    sb->buffer->color = col;
    sb->buffer++;

    sb->buffer->position = new_pos[2];
    sb->buffer->uv = uv;
    sb->buffer->tex_id = tid;
    sb->buffer->color = col;
    sb->buffer++;

    sb->buffer->position = new_pos[3];
    sb->buffer->uv = uv;
    sb->buffer->tex_id = tid;
    sb->buffer->color = col;
    sb->buffer++;

    sb->index_count += 6;
    sb->num_sprites++;
}
