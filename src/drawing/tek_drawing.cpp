#include "tek_drawing.hpp"

#include "../platform/tek_platform.hpp"

#include <string.h>
#include <stdio.h>

namespace tek
{

static TekSpritebatch g_sprite_batch;

static TekRenderStats g_stats;
static u32 g_drawcalls = 0;

static TekShader g_sprite_shader;
static TekShader g_shape_shader;
static TekShader g_geometry_shader;
static TekShader g_billboard_shader;
static TekShader g_meshbuffer_shader;

static u32 g_width;
static u32 g_height;

void tek_renderer_destroy()
{
	tek_sb_destroy(&g_sprite_batch);
	tek_shader_destroy(&g_sprite_shader);
	tek_shader_destroy(&g_shape_shader);
	tek_shader_destroy(&g_geometry_shader);
	tek_shader_destroy(&g_billboard_shader);
	tek_shader_destroy(&g_meshbuffer_shader);
}

bool tek_renderer_init(u32 width, u32 height)
{
	load_gl_extensions();

	printf("!!!=================================================!!!\n");
	printf("!!!=== OpenGL Stats ================================!!!\n");
	printf("!!!=================================================!!!\n");

	GLCall(print_gl_string(GL_RENDERER, "GL_RENDERER"));
	GLCall(print_gl_string(GL_VENDOR, "GL_VENDOR"));

	GLCall(print_gl_string(GL_SHADING_LANGUAGE_VERSION, "GL_SHADING_LANGUAGE_VERSION"));
	GLCall(print_gl_string(GL_VERSION, "GL_VERSION"));

	printf("Rendering Resolution: %dx%d\n", width, height);

	g_width = width;
	g_height = height;

	GLCall(glViewport(0, 0, width, height));
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));

	GLCall(glEnable(GL_DEPTH_TEST));

	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	GLCall(glFrontFace(GL_CCW));

	GLCall(glCullFace(GL_BACK));

	GLCall(glEnable(GL_CULL_FACE));

	GLCall(glLineWidth(3));

	if (!tek_sb_init(&g_sprite_batch, width, height))
	{
		return false;
	}

	if (!tek_shader_load(&g_sprite_shader, "basic.glsl"))
		return false;


	if (!tek_shader_load(&g_shape_shader, "shape.glsl"))
		return false;

	if (!tek_shader_load(&g_geometry_shader, "geometry.glsl"))
		return false;

	if (!tek_shader_load(&g_billboard_shader, "billboard.glsl"))
		return false;

	if (!tek_shader_load(&g_meshbuffer_shader, "meshbuffer.glsl"))
		return false;

	return true;
}

void tek_renderer_start_frame()
{
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void tek_renderer_end_frame()
{
	g_stats.num_drawcalls = g_drawcalls + g_sprite_batch.drawcalls;
	g_stats.num_sprites = g_sprite_batch.num_sprites;

	g_drawcalls = 0;

	tek_sb_reset_stats(&g_sprite_batch);
}

void tek_renderer_resize(u32 width, u32 height)
{
	g_width = width;
	g_height = height;
	//GLCall(glViewport(0, 0, width, height));

	tek_sb_resize(&g_sprite_batch, width, height);
}

void tek_renderer_bind_framebuffer(TekFramebuffer* buffer, TekColor color)
{
	tek_fb_bind_writing(buffer);
	GLCall(glViewport(0, 0, buffer->width, buffer->height));
	Vec4 col = color.to_vec4();
	GLCall(glClearColor(col.x, col.y, col.z, col.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void tek_renderer_unbind_framebuffer()
{
	tek_fb_unbind();
	//GLCall(glViewport(0, 0, g_width, g_height));
}

void tek_renderer_viewport(u32 width, u32 height)
{
	GLCall(glViewport(0, 0, width, height));
}

void tek_renderer_disable_depth_test()
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void tek_renderer_enable_depth_test()
{
	GLCall(glEnable(GL_DEPTH_TEST));
}

void tek_renderer_clear(TekColor color)
{
	Vec4 col = color.to_vec4();
	GLCall(glClearColor(col.x, col.y, col.z, col.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

TekSpritebatch* tek_renderer_get_sb()
{
	return &g_sprite_batch;
}

TekRenderStats* tek_renderer_get_stats()
{
	//g_stats.num_sprites = g_sprite_batch->num_sprites;
	return &g_stats;
}

struct TekVertexData
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
};

TekVertexData tek_mesh_vertex_data_create(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
    TekVertexData res;
    res.position = vec3_create(x, y, z);
    res.uv = vec2_create(u, v);
    res.normal = vec3_create(nx, ny, nz);
    return res;
}

void tek_renderer_render_sprite(TekRect dest, TekRect src, u32 texture_id, Mat4* ortho)
{
	tek_shader_bind(&g_sprite_shader);

	tek_shader_uniform_mat4(&g_sprite_shader, "u_mvp", ortho, 1, false);

	tek_shader_uniform_int(&g_sprite_shader, "u_use_texture", 1);

	TekVertexData vertices[4];
	u32 indices[6];

	float x = dest.x;
	float y = dest.y;
	float w = dest.x + dest.w;
	float h = dest.y + dest.h;

	Vec2 uv[4];
	uv[0].x = src.x; uv[0].y = src.h;
	uv[1].x = src.x; uv[1].y = src.y;
	uv[2].x = src.w, uv[2].y = src.y;
	uv[3].x = src.w, uv[3].y = src.h;

	vertices[0] = tek_mesh_vertex_data_create(x, y, 0, uv[0].x, uv[0].y, 0, 0, 0);
	vertices[1] = tek_mesh_vertex_data_create(x, h, 0, uv[1].x, uv[1].y, 0, 0, 0);
	vertices[2] = tek_mesh_vertex_data_create(w, h, 0, uv[2].x, uv[2].y, 0, 0, 0);
	vertices[3] = tek_mesh_vertex_data_create(w, y, 0, uv[3].x, uv[3].y, 0, 0, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	int size = 4 * sizeof(TekVertexData);

	u32 vao;
	u32 vbo;
	u32 ibo;

	GLCall(glGenVertexArrays(1, &vao));

	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));
	GLCall(glGenBuffers(1, &ibo));

	int loc_pos = tek_shader_get_attrib_loc(&g_sprite_shader, "a_position");
	int loc_uv = tek_shader_get_attrib_loc(&g_sprite_shader, "a_uv");

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, sizeof(TekVertexData), (const GLvoid*)(offsetof(TekVertexData, position))));
	GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, sizeof(TekVertexData), (const GLvoid*)(offsetof(TekVertexData, uv))));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), indices, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(loc_pos));
	GLCall(glEnableVertexAttribArray(loc_uv));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_uv));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glDeleteBuffers(1, &vbo));
	GLCall(glDeleteBuffers(1, &ibo));
	GLCall(glDeleteVertexArrays(1, &vao));
}

TekShader* tek_renderer_get_shape_shader()
{
	return &g_shape_shader;
}

TekShader* tek_renderer_get_geometry_shader()
{
	return &g_geometry_shader;
}

TekShader* tek_renderer_get_billboard_shader()
{
	return &g_billboard_shader;
}

TekShader* tek_renderer_get_meshbuffer_shader()
{
	return &g_meshbuffer_shader;
}
}
