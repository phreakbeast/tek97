#ifndef _TEK_COLOR_HPP
#define _TEK_COLOR_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

class Color
{
public:
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    Color();
    Color(u8 r, u8 g, u8 b);
    Color(u8 r, u8 g, u8 b, u8 a);

    const u32 to_int() const;

    const Vec3 to_vec3() const;

    const Vec4 to_vec4() const;

    const Color lighten(u8 shade) const;

    const Color darken(u8 shade) const;

    const Color lerp(Color c2, float t) const;

    static const u32 floats_to_int(float r, float g, float b, float a);

    static const u32 vec4_to_int(Vec4 color);
    
    static const u32 vec3_to_int(Vec3 color);

    static const Color white();

    static const Color black();

    static const Color red();

    static const Color green();

    static const Color blue();

    static const Color yellow();

    static const Color random();
};

#endif
