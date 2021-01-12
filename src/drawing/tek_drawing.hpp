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
#include "tek_mesh.hpp"
#include "tek_camera.hpp"

typedef struct
{
    float delta;
    u32 fps;
    u32 ups;
    u32 num_sprites;
    u32 num_drawcalls;
}TekRenderStats;


bool renderer_init(u32 width, u32 height);

void renderer_destroy();
    
void renderer_start_frame();

void renderer_end_frame();

void renderer_bind_framebuffer(TekFramebuffer *buffer, Color color);

void renderer_unbind_framebuffer();

void renderer_viewport(u32 width, u32 height);

void renderer_disable_depth_test();

void renderer_enable_depth_test();

void renderer_clear(Color color);

TekRenderStats* renderer_get_stats();

TekSpritebatch* renderer_get_sb();

Shader* renderer_get_geometry_shader();

void renderer_resize(u32 width, u32 height);

void renderer_render_sprite(TekRect dest, TekRect src, u32 texture_id, Mat4 *ortho);

void renderer_render_mesh(TekMesh* mesh, u32 texture_id, Mat4* transform, TekCamera* cam);


#endif
