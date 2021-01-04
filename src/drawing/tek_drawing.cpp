#include "tek_drawing.hpp"

#include "../platform/tek_platform.hpp"

#include <string.h>
#include <stdio.h>

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

static void upload_material(TekMaterial* mat, TekShader* shader);

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
	Vec4 col = tek_color_to_vec4(color);
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
	Vec4 col = tek_color_to_vec4(color);
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

void tek_renderer_render_sprite(TekRect dest, TekRect src, u32 texture_id, Mat4* ortho)
{
	tek_shader_bind(&g_sprite_shader);

	tek_shader_uniform_mat4(&g_sprite_shader, "u_mvp", ortho, 1);

	tek_shader_uniform_int(&g_sprite_shader, "u_use_texture", 1);

	TekMeshVertexData vertices[4];
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

	int size = 4 * sizeof(TekMeshVertexData);

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

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, sizeof(TekMeshVertexData), (const GLvoid*)(offsetof(TekMeshVertexData, position))));
	GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, sizeof(TekMeshVertexData), (const GLvoid*)(offsetof(TekMeshVertexData, uv))));

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

void tek_renderer_draw_mesh(TekMesh* mesh, TekMaterial* material, Mat4* transform, TekCamera* cam,
	TekDirectionalLight* light, TekPointLight* plights, u32 num_plights)
{
	tek_shader_bind(&g_geometry_shader);

	upload_material(material, &g_geometry_shader);

	Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
	Mat4 world;
	Mat4 mv;
	if (transform != NULL)
	{
		mvp = mat4_mul2(&mvp, transform);
		world = mat4_transposed(transform);
		mv = mat4_mul2(&cam->view, transform);
	}
	else
	{
		world = mat4_transposed(&world);
		mv = cam->view;
	}

	mvp = mat4_transposed(&mvp);
	mv = mat4_transposed(&mv);


	tek_shader_uniform_mat4(&g_geometry_shader, "u_mvp", &mvp, 1);
	tek_shader_uniform_mat4(&g_geometry_shader, "u_world", &world, 1);
	tek_shader_uniform_mat4(&g_geometry_shader, "u_mv", &mv, 1);

	if (light)
	{
		tek_shader_uniform_vec3(&g_geometry_shader,"u_direct_light.direction", light->direction);
		tek_shader_uniform_vec3(&g_geometry_shader,"u_direct_light.color", light->color);
		tek_shader_uniform_float(&g_geometry_shader,"u_direct_light.intensity", light->intensity);
		//tek_shader_uniform_vec3(&g_geometry_shader,"u_eye_world_pos", cam->position);
		tek_shader_uniform_int(&g_geometry_shader,"u_use_d_light", 1);
	}
	else
	{
		tek_shader_uniform_int(&g_geometry_shader, "u_use_d_light", 0);
	}

	if (plights)
	{
		tek_shader_uniform_int(&g_geometry_shader, "u_use_p_light", 1);
		tek_shader_uniform_int(&g_geometry_shader, "u_num_pointlights", num_plights);
		for (u32 i = 0; i < num_plights; ++i)
		{
			char name0[128];
			char name1[128];
			char name2[128];

			sprintf(name0, "u_point_lights[%u].position", i);
			sprintf(name1, "u_point_lights[%u].color", i);
			sprintf(name2, "u_point_lights[%u].range", i);

			tek_shader_uniform_vec3(&g_geometry_shader, name0, plights[i].position);
			tek_shader_uniform_vec3(&g_geometry_shader, name1, plights[i].color);
			tek_shader_uniform_float(&g_geometry_shader, name2, plights[i].range);
		}
	}
	else
	{
		tek_shader_uniform_int(&g_geometry_shader, "u_use_p_light", 0);
	}

	tek_mesh_render(mesh);

	g_drawcalls++;
}

void tek_renderer_draw_shape(TekShape* shape, Mat4* transform, TekCamera* cam)
{
	tek_shader_bind(&g_shape_shader);

	Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
	if (transform != NULL)
	{
		mvp = mat4_mul2(&mvp, transform);
	}

	mvp = mat4_transposed(&mvp);

	tek_shader_uniform_mat4(&g_shape_shader,"u_mvp", &mvp, 1);

	tek_shape_render(shape);
	g_drawcalls++;
}

void tek_renderer_draw_billboard(TekBillboard* billboard, TekMaterial* material, Mat4* transform, TekCamera* cam, bool spherical)
{
	tek_shader_bind(&g_billboard_shader);

	upload_material(material, &g_billboard_shader);

	Mat4 p = mat4_transposed(&cam->projection);
	Mat4 mv = cam->view;
	if (transform != NULL)
	{
		mv = mat4_mul2(&cam->view, transform);
	}
	mv = mat4_transposed(&mv);

	tek_shader_uniform_mat4(&g_billboard_shader,"u_mv", &mv, 1);
	tek_shader_uniform_mat4(&g_billboard_shader,"u_p", &p, 1);

	int sph = 0;
	if (spherical)
		sph = 1;

	tek_shader_uniform_int(&g_billboard_shader,"u_spherical", sph);

	//tek_bb_render(billboard);

	g_drawcalls++;
}

static void upload_material(TekMaterial* mat, TekShader* shader)
{
	if (mat->has_diffuse_map)
	{
		tek_shader_uniform_int(shader,"u_material.has_diffuse_map", 1);
		tek_tex_bind(&mat->diffuse_map, 0);
	}
	else
	{
		tek_shader_uniform_int(shader, "u_material.has_diffuse_map", 0);
		tek_shader_uniform_vec4(shader,"u_material.ambient_color", tek_color_to_vec4(mat->ambient_color));
		tek_shader_uniform_vec4(shader,"u_material.diffuse_color", tek_color_to_vec4(mat->diffuse_color));
	}

	/*
	if (mat->has_specular_map)
	{
		tek_shader_uniform_int(shader,"u_material.has_specular_map", 1);
		tek_tex_bind(&mat->specular_map, 1);
	}
	else
	{
		tek_shader_uniform_int(shader, "u_material.has_specular_map", 0);
		tek_shader_uniform_vec4(shader,"u_material.specular_color", tek_color_to_vec4(mat->specular_color));
	}
	*/
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