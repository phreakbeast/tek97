#ifndef _TEK_SHADER_HPP
#define _TEK_SHADER_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "../platform/tek_platform.hpp"

#define MAX_ATTRIBS 16
#define MAX_UNIFORMS 64

struct TekLocation
{
    int loc;
    char name[128];
};

class Shader
{
public:
    Shader();
    ~Shader();
        
    void bind();

    void uniform_int(const char* name, int val);

    void uniform_int_array(const char* name, int *val, int count);

    void uniform_float(const char* name, float val);

    void uniform_float_array(const char* name, float *val, int count);

    void uniform_vec2(const char* name, Vec2 val);

    void uniform_vec3(const char* name, Vec3 val);

    void uniform_vec4(const char* name, Vec4 val);

    void uniform_mat4(const char* name, Mat4 *val, u32 count, bool transpose = false);

    static Shader* load(const char *filename);

    int get_attrib_loc(const char* name);
    int get_uniform_loc(const char* name);

private:
    u32 program;
    static u32 cur_program;
    
    TekLocation attribs[MAX_ATTRIBS];
    TekLocation uniforms[MAX_UNIFORMS];
};


#endif
