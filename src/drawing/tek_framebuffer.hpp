#ifndef _TEK_FRAMEBUFFER_HPP
#define _TEK_FRAMEBUFFER_HPP

#include "../core/tek_core.hpp"
#include "tek_rect.hpp"
#include "../math/tek_math.hpp"

namespace tek
{

typedef struct
{
    u32 fbo;
    u32 tex_id;
    u32 depth_id;

    u32 width;
    u32 height;
}TekFramebuffer;

void tek_fb_init(TekFramebuffer* fb, u32 width, u32 height);

void tek_fb_destroy(TekFramebuffer* fb);

void tek_fb_bind_reading(TekFramebuffer* fb, u32 texture_unit);

void tek_fb_bind_writing(TekFramebuffer* fb);

void tek_fb_unbind();

}
#endif
