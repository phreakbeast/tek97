#ifndef _TEK_SHADER_HPP
#define _TEK_SHADER_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "../platform/tek_platform.hpp"

#define MAX_ATTRIBS 16
#define MAX_UNIFORMS 64

typedef struct
{
	int loc;
	char name[128];
}TekLocation;

typedef struct
{
	u32 program;

	TekLocation attribs[MAX_ATTRIBS];
	TekLocation uniforms[MAX_UNIFORMS];
} TekShader;

void tek_shader_destroy(TekShader *shader);

void tek_shader_bind(TekShader *shader);

void tek_shader_uniform_int(TekShader* shader, const char* name, int val);

void tek_shader_uniform_int_array(TekShader* shader, const char* name, int *val, int count);

void tek_shader_uniform_float(TekShader* shader, const char* name, float val);

void tek_shader_uniform_float_array(TekShader* shader, const char* name, float *val, int count);

void tek_shader_uniform_vec2(TekShader* shader, const char* name, Vec2 val);

void tek_shader_uniform_vec3(TekShader* shader, const char* name, Vec3 val);

void tek_shader_uniform_vec4(TekShader* shader, const char* name, Vec4 val);

void tek_shader_uniform_mat4(TekShader* shader, const char* name, Mat4 *val, u32 count, bool transpose = false);

bool tek_shader_load(TekShader *shader, const char *filename);

int tek_shader_get_attrib_loc(TekShader* shader, const char* name);
int tek_shader_get_uniform_loc(TekShader* shader, const char* name);

#endif
