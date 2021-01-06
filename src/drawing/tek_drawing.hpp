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

typedef struct
{
    float delta;
    u32 fps;
    u32 ups;
    u32 num_sprites;
    u32 num_drawcalls;
} TekRenderStats;

bool tek_renderer_init(u32 width, u32 height);

void tek_renderer_destroy();

void tek_renderer_start_frame();

void tek_renderer_end_frame();

void tek_renderer_bind_framebuffer(TekFramebuffer *buffer, TekColor color);

void tek_renderer_unbind_framebuffer();

void tek_renderer_viewport(u32 width, u32 height);

void tek_renderer_disable_depth_test();

void tek_renderer_enable_depth_test();

void tek_renderer_clear(TekColor color);

TekRenderStats *tek_renderer_get_stats();

TekSpritebatch *tek_renderer_get_sb();

TekShader* tek_renderer_get_shape_shader();
TekShader* tek_renderer_get_geometry_shader();
TekShader* tek_renderer_get_billboard_shader();
TekShader* tek_renderer_get_meshbuffer_shader();

void tek_renderer_resize(u32 width, u32 height);

void tek_renderer_render_sprite(TekRect dest, TekRect src, u32 texture_id, Mat4* ortho);

}

#endif
