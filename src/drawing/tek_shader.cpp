#include "tek_shader.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace tek
{

static u32 g_cur_program = 0;

void tek_shader_destroy(TekShader *shader)
{
	GLCall(glDeleteProgram(shader->program));
}

void tek_shader_bind(TekShader *shader)
{	
	if (g_cur_program != shader->program)
	{
		GLCall(glUseProgram(shader->program));
		g_cur_program = shader->program;
	}
}

void tek_shader_uniform_int(TekShader* shader, const char* name, int val)
{
	 
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform1i(loc, val));
}

void tek_shader_uniform_int_array(TekShader* shader, const char* name, int *val, int count)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform1iv(loc, count, val));
}

void tek_shader_uniform_float(TekShader* shader, const char* name, float val)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform1f(loc, val));
}

void tek_shader_uniform_float_array(TekShader* shader, const char* name, float *val, int count)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform1fv(loc, count, val));
}

void tek_shader_uniform_vec2(TekShader* shader, const char* name, Vec2 val)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform2f(loc, val.x, val.y));
}

void tek_shader_uniform_vec3(TekShader* shader, const char* name, Vec3 val)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform3f(loc, val.x, val.y, val.z));
}

void tek_shader_uniform_vec4(TekShader* shader, const char* name, Vec4 val)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	GLCall(glUniform4f(loc, val.x, val.y, val.z, val.z));
}

void tek_shader_uniform_mat4(TekShader* shader, const char* name, Mat4 *val, u32 count, bool transpose)
{
	int loc = tek_shader_get_uniform_loc(shader, name);
	if (transpose)
	{
		GLCall(glUniformMatrix4fv(loc, count, GL_TRUE, &val->m[0]));
	}
	else
	{
		GLCall(glUniformMatrix4fv(loc, count, GL_FALSE, &val->m[0]));
	}
	
}

static GLuint create_shader(const char *src, GLenum type)
{
	switch (type)
	{
		case GL_VERTEX_SHADER:
			printf("compiling vertex shader ...\n");
			break;
		case GL_FRAGMENT_SHADER:
			printf("compiling fragment shader ...\n");
			break;
		case GL_GEOMETRY_SHADER:
			printf("compiling geometry shader ...\n");
			break;
		default:
		{
			printf("ERROR: Invalid Shader type\n");
			return 0;
		}
	}

	GLCall(GLuint id = glCreateShader(type));

	const char *c_src = src;
	GLCall(glShaderSource(id, 1, &c_src, NULL));
	GLCall(glCompileShader(id));

	GLCall(GLint result = GL_FALSE);
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result)
	{
		int info_length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
		char *error_message = (char*)tek_malloc(sizeof(char) * info_length + 1);
		GLCall(glGetShaderInfoLog(id, info_length, NULL, &error_message[0]));
		printf("ERROR: Shader compilation error: %s \n", error_message);
		GLCall(glDeleteShader(id));
		tek_free(error_message);
		return 0;
	}

	return id;
}

static GLuint link_shader(GLuint vert_id, GLuint frag_id, GLuint geom_id)
{
	printf("linking shader ...\n");
	GLCall(GLuint program = glCreateProgram());

	GLCall(glAttachShader(program, vert_id));
	GLCall(glAttachShader(program, frag_id));
	if (geom_id > 0){
	    GLCall(glAttachShader(program, geom_id));
	}

	GLCall(glLinkProgram(program));
	GLint result = GL_FALSE;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (!result)
	{
		int info_length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length));
		char *error_message = (char*)tek_malloc(sizeof(char) * info_length + 1);
		GLCall(glGetProgramInfoLog(program, info_length, NULL, &error_message[0]));
		printf("ERROR: Shader linking error: %s\n", error_message);
		GLCall(glDeleteProgram(program));
		tek_free(error_message);
		return 0;
	}

	return program;
}

static char *include_source(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error: cannot open file %s\n", filename);
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	u64 buffsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	char *line = (char*)tek_malloc(sizeof(char) * buffsize + 1);
	size_t new_len = fread(line, sizeof(char), buffsize, fp);

	line[new_len++] = '\0';

	fclose(fp);

	return line;
}

typedef enum
{
	SOURCE_GEOMETRY = 0,
	SOURCE_VERTEX,
	SOURCE_FRAGMENT,
	NUM_SOURCE_TYPES
} SourceType;

typedef struct
{
	char *src;
} ShaderSource;

void shader_source_add(ShaderSource *src, const char *line)
{
	size_t len = strlen(line);
	if (src->src == NULL)
	{
		src->src = (char*)tek_malloc(sizeof(char) * (len + 1));
		memcpy(src->src, line, sizeof(char) * len);
		src->src[len] = '\0';
	}
	else
	{
		size_t old_len = strlen(src->src);
		src->src = (char*)tek_realloc(src->src, sizeof(char) * (old_len + len + 1));
		memcpy(&src->src[old_len],line,sizeof(char) * len);
		src->src[old_len + len] = '\0';
	}
}

static bool load_source(const char *filename, ShaderSource *vert_src, ShaderSource *frag_src, ShaderSource *geom_src)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error: cannot open file %s\n", filename);
		return false;
	}

	SourceType source_type = SOURCE_VERTEX;

	char line[1024];
	while (fgets(line, 1024, fp))
	{
		if (line[0] == '#' && line[1] == 'i' && line[2] == 'n')
		{
			char file[64];
			int res = sscanf(line, "#in %s", file);
			if (res != 1)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return 0;
			}

			char path[128];
			sprintf(path, "%s/%s", "shaders", file);

			char *inc = include_source(path);
			if (source_type == SOURCE_VERTEX)
			{
				shader_source_add(vert_src, inc);
			}
			else if (source_type == SOURCE_FRAGMENT)
			{
				shader_source_add(frag_src, inc);
			}
			else if (source_type == SOURCE_GEOMETRY)
			{
				shader_source_add(geom_src, inc);
			}
			tek_free(inc);
		}
		else if (line[0] == '#' && line[1] == 'g' && line[2] == 's')
		{
			source_type = SOURCE_GEOMETRY;
		}
		else if (line[0] == '#' && line[1] == 'v' && line[2] == 's')
		{
			source_type = SOURCE_VERTEX;
		}
		else if (line[0] == '#' && line[1] == 'f' && line[2] == 's')
		{
			source_type = SOURCE_FRAGMENT;
		}
		else
		{
			if (source_type == SOURCE_VERTEX)
			{
				shader_source_add(vert_src, line);
			}
			else if (source_type == SOURCE_FRAGMENT)
			{
				shader_source_add(frag_src, line);
			}
			else if (source_type == SOURCE_GEOMETRY)
			{
				shader_source_add(geom_src, line);
			}

		}
	}
	fclose(fp);

	return true;
}


bool tek_shader_load(TekShader *shader, const char *filename)
{
	printf("loading shader %s\n", filename);

	char path[128];
	sprintf(path, "%s%s", "shaders/", filename);
	//printf("shader path %s\n", path);


	ShaderSource vert_src = {NULL};
	ShaderSource frag_src = {NULL};
	ShaderSource geom_src = {NULL};
	if (!load_source(path, &vert_src, &frag_src, &geom_src))
	{
		if (vert_src.src != NULL)
		{
			tek_free(vert_src.src);
		}
		if (frag_src.src != NULL)
		{
			tek_free(frag_src.src);
		}
		if (geom_src.src != NULL)
		{
			tek_free(geom_src.src);
		}
		return false;
	}


	u32 vert_id = create_shader(vert_src.src, GL_VERTEX_SHADER);
	assert(vert_id != 0);
	u32 frag_id = create_shader(frag_src.src, GL_FRAGMENT_SHADER);
	//printf("%s\n", frag_src.c_str());
	assert(frag_id != 0);

	u32 geom_id = 0;
	if (geom_src.src != NULL)
	{
		geom_id = create_shader(geom_src.src, GL_GEOMETRY_SHADER);
		assert(geom_id != 0);
	}

	shader->program = link_shader(vert_id, frag_id, geom_id);
	assert(shader->program > 0);

	if (vert_src.src != NULL)
	{
		tek_free(vert_src.src);
	}
	if (frag_src.src != NULL)
	{
		tek_free(frag_src.src);
	}
	if (geom_src.src != NULL)
	{
		tek_free(geom_src.src);
	}


	for (u32 i = 0; i < MAX_ATTRIBS; ++i)
	{
		shader->attribs[i].name[0] = '\0';
		shader->attribs[i].loc = -1;;
	}

	for (u32 i = 0; i < MAX_UNIFORMS; ++i)
	{
		shader->uniforms[i].name[0] = '\0';
		shader->uniforms[i].loc = -1;;
	}

	return true;
}

int tek_shader_get_attrib_loc(TekShader* shader, const char* name)
{
	for (u32 i = 0; i < MAX_ATTRIBS; ++i)
	{
		if (strcmp(shader->attribs[i].name, name) == 0)
		{
			return shader->attribs[i].loc;
		}
	}

	//not found
	int loc = GLCall(glGetAttribLocation(shader->program, name));
	if (loc >= 0)
	{
		//add to list
		for (u32 i = 0; i < MAX_ATTRIBS; ++i)
		{
			//find free slot
			if (shader->attribs[i].name[0] == '\0')
			{
				strcpy(shader->attribs[i].name, name);
				shader->attribs[i].loc = loc;
				return loc;
			}
		}
	}

	//no location with that name
#ifdef TEK_DEBUG
	printf("Warning: attrib %s not found\n", name);
#endif
	return -1;
}

int tek_shader_get_uniform_loc(TekShader* shader, const char* name)
{
	for (u32 i = 0; i < MAX_UNIFORMS; ++i)
	{
		if (strcmp(shader->uniforms[i].name, name) == 0)
		{
			return shader->uniforms[i].loc;
		}
	}

	//not found
	int loc = GLCall(glGetUniformLocation(shader->program, name));
	if (loc >= 0)
	{
		//add to list
		for (u32 i = 0; i < MAX_UNIFORMS; ++i)
		{
			//find free slot
			if (shader->uniforms[i].name[0] == '\0')
			{
				strcpy(shader->uniforms[i].name, name);
				shader->uniforms[i].loc = loc;
				return loc;
			}
		}
	}

	//no location with that name
#ifdef TEK_DEBUG
	printf("Warning: uniform %s not found\n", name);
#endif
	return -1;
}
}
