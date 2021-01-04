#include "tek_meshbuffer.hpp"

#include "tek_drawing.hpp"
#include <string.h>

TekMBVertexData tek_mb_vertex_create(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
	TekMBVertexData res;
	res.position = vec3_create(x, y, z);
	res.uv = vec2_create(u, v);
	res.normal = vec3_create(nx, ny, nz);
	return res;
}

void tek_mb_init(TekMeshbuffer* mb, TekCamera* cam, TekLightManager* lights, TekOrderType order)
{
	mb->cam = cam;
	mb->lights = lights;
	mb->num_commands = 0;
	mb->command_capacity = 100;
	mb->commands = (DrawCommand*)tek_malloc(sizeof(DrawCommand) * mb->command_capacity);

	mb->vertex_buffer_size = 0;
	mb->vertex_buffer_capacity = 100;
	mb->vertex_buffer = (TekMBVertexData*)tek_malloc(sizeof(TekMBVertexData) * mb->vertex_buffer_capacity);
}

void tek_mb_destroy(TekMeshbuffer* mb)
{
	tek_free(mb->commands);
	tek_free(mb->vertex_buffer);
}

static void add_command(TekMeshbuffer* mb, DrawCommand cmd)
{
	if (mb->num_commands >= mb->command_capacity)
	{
		mb->command_capacity += mb->command_capacity;
		mb->commands = (DrawCommand*)tek_realloc(mb->commands, sizeof(DrawCommand) * mb->command_capacity);
	}
	mb->commands[mb->num_commands++] = cmd;
}

static Vec2 add_material(TekMeshbuffer* mb, TekMaterial* material)
{
	//check if texture exists
	u32 tex_id = 0;
	if (material->has_diffuse_map)
	{
		bool found = false;
		for (u32 i = 0; i < mb->num_texture_slots; ++i)
		{
			if (mb->texture_slots[i]->id == material->diffuse_map.id)
			{
				found = true;
				tex_id = i;
				break;
			}
		}
		if (!found)
		{
			//check if there is room for a texture map
			if (mb->num_texture_slots >= 8)
			{
				tek_mb_render(mb);
				tek_mb_reset(mb);
			}
			mb->texture_slots[mb->num_texture_slots++] = &material->diffuse_map;
			tex_id = mb->num_texture_slots - 1;
		}
	}


	float mat_id = 0;
	//check if material exist
	bool found = false;
	for (u32 i = 0; i < mb->num_material_slots; ++i)
	{
		if (mb->material_slots[i].material == material)
		{
			mat_id = i;
			found = true;
			break;
		}
	}
	if (!found)
	{
		//check if there is room for a material
		if (mb->num_material_slots >= 8)
		{
			tek_mb_render(mb);
			tek_mb_reset(mb);
		}
		MaterialCommand cmd;
		cmd.material = material;
		cmd.tex_id = tex_id;
		mb->material_slots[mb->num_material_slots++] = cmd;
		mat_id = mb->num_material_slots - 1;
	}

	return vec2_create(mat_id, tex_id);
}

void tek_mb_draw_mesh(TekMeshbuffer* mb, TekMesh* mesh, TekMaterial* material, TekTransform* transform)
{
	//early check draw distance

	Mat4 trans = transform->matrix;
	trans = mat4_transposed(&trans);

	Vec2 ids = add_material(mb,material);
	float mat_id = ids.x;
	float tex_id = ids.y;
	
	for (u32 i = 0; i < mesh->num_indices; i += 3)
	{
		TekMeshVertexData v0 = mesh->vertices[mesh->indices[i + 0]];
		TekMeshVertexData v1 = mesh->vertices[mesh->indices[i + 1]];
		TekMeshVertexData v2 = mesh->vertices[mesh->indices[i + 2]];

		DrawCommand dc;
		TekMBVertexData vert0;
		TekMBVertexData vert1;
		TekMBVertexData vert2;
		vert0.position = v0.position;
		vert1.position = v1.position;
		vert2.position = v2.position;
		vert0.normal = v0.normal;
		vert1.normal = v1.normal;
		vert2.normal = v2.normal;
		vert0.uv = v0.uv;
		vert1.uv = v1.uv;
		vert2.uv = v2.uv;
		vert0.model0 = trans.rows[0];
		vert0.model1 = trans.rows[1];
		vert0.model2 = trans.rows[2];
		vert0.model3 = trans.rows[3];
		vert1.model0 = trans.rows[0];
		vert1.model1 = trans.rows[1];
		vert1.model2 = trans.rows[2];
		vert1.model3 = trans.rows[3];
		vert2.model0 = trans.rows[0];
		vert2.model1 = trans.rows[1];
		vert2.model2 = trans.rows[2];
		vert2.model3 = trans.rows[3];

		vert0.flags = vec4_create(mat_id,0,0,0);
		vert1.flags = vec4_create(mat_id,0,0,0);
		vert2.flags = vec4_create(mat_id,0,0,0);

		dc.vertices[0]= vert0;
		dc.vertices[1]= vert1;
		dc.vertices[2]= vert2;
		
		dc.tex_id = tex_id;
		dc.mat_id = mat_id;
		
		dc.transform = transform->matrix;

		dc.distance = vec3_distance(mb->cam->position, transform->position);

		//mb->commands.push_back(dc);
		add_command(mb, dc);
	}	
	
}

void tek_mb_draw_vertices(TekMeshbuffer* mb, TekMeshVertexData* vertices, u32 num_vertices, TekMaterial* material)
{
	Mat4 trans;
	mat4_init(&trans);
	trans = mat4_transposed(&trans);

	Vec2 ids = add_material(mb, material);
	float mat_id = ids.x;
	float tex_id = ids.y;

	for (u32 i = 0; i < num_vertices; i += 3)
	{
		TekMeshVertexData v0 = vertices[i + 0];
		TekMeshVertexData v1 = vertices[i + 1];
		TekMeshVertexData v2 = vertices[i + 2];

		DrawCommand dc;
		TekMBVertexData vert0;
		TekMBVertexData vert1;
		TekMBVertexData vert2;
		vert0.position = v0.position;
		vert1.position = v1.position;
		vert2.position = v2.position;
		vert0.normal = v0.normal;
		vert1.normal = v1.normal;
		vert2.normal = v2.normal;
		vert0.uv = v0.uv;
		vert1.uv = v1.uv;
		vert2.uv = v2.uv;
		vert0.model0 = trans.rows[0];
		vert0.model1 = trans.rows[1];
		vert0.model2 = trans.rows[2];
		vert0.model3 = trans.rows[3];
		vert1.model0 = trans.rows[0];
		vert1.model1 = trans.rows[1];
		vert1.model2 = trans.rows[2];
		vert1.model3 = trans.rows[3];
		vert2.model0 = trans.rows[0];
		vert2.model1 = trans.rows[1];
		vert2.model2 = trans.rows[2];
		vert2.model3 = trans.rows[3];

		vert0.flags = vec4_create(mat_id,0,0,0);
		vert1.flags = vec4_create(mat_id,0,0,0);
		vert2.flags = vec4_create(mat_id,0,0,0);

		dc.vertices[0]= vert0;
		dc.vertices[1]= vert1;
		dc.vertices[2]= vert2;

		dc.tex_id = tex_id;
		dc.mat_id = mat_id;

		dc.transform = Mat4();

		dc.distance = vec3_distance(mb->cam->position, vert0.position);

		//mb->commands.push_back(dc);
		add_command(mb, dc);
	}
}

void tek_mb_draw_billboard(TekMeshbuffer* mb, TekBillboard* bb, TekMaterial* material, TekTransform* transform, bool spherical)
{
	Mat4 trans = transform->matrix;
	trans = mat4_transposed(&trans);

	Vec2 ids = add_material(mb, material);
	float mat_id = ids.x;
	float tex_id = ids.y;

	for (u32 i = 0; i < 6; i += 3)
	{
		TekMeshVertexData v0 = bb->vertices[i + 0];
		TekMeshVertexData v1 = bb->vertices[i + 1];
		TekMeshVertexData v2 = bb->vertices[i + 2];

		DrawCommand dc;
		TekMBVertexData vert0;
		TekMBVertexData vert1;
		TekMBVertexData vert2;
		vert0.position = v0.position;
		vert1.position = v1.position;
		vert2.position = v2.position;
		vert0.normal = v0.normal;
		vert1.normal = v1.normal;
		vert2.normal = v2.normal;
		vert0.uv = v0.uv;
		vert1.uv = v1.uv;
		vert2.uv = v2.uv;
		vert0.model0 = trans.rows[0];
		vert0.model1 = trans.rows[1];
		vert0.model2 = trans.rows[2];
		vert0.model3 = trans.rows[3];
		vert1.model0 = trans.rows[0];
		vert1.model1 = trans.rows[1];
		vert1.model2 = trans.rows[2];
		vert1.model3 = trans.rows[3];
		vert2.model0 = trans.rows[0];
		vert2.model1 = trans.rows[1];
		vert2.model2 = trans.rows[2];
		vert2.model3 = trans.rows[3];

		float sphere = 1;
		if (spherical)
			sphere = 2;

		vert0.flags = vec4_create(mat_id, sphere, 0, 0);
		vert1.flags = vec4_create(mat_id, sphere, 0, 0);
		vert2.flags = vec4_create(mat_id, sphere, 0, 0);

		dc.vertices[0] = vert0;
		dc.vertices[1] = vert1;
		dc.vertices[2] = vert2;

		dc.tex_id = tex_id;
		dc.mat_id = mat_id;

		dc.transform = transform->matrix;

		dc.distance = vec3_distance(mb->cam->position, transform->position);

		//mb->commands.push_back(dc);
		add_command(mb, dc);
	}
}

static void add_vertex(TekMeshbuffer* mb, TekMBVertexData vert)
{
	if (mb->vertex_buffer_size >= mb->vertex_buffer_capacity)
	{
		mb->vertex_buffer_capacity += mb->vertex_buffer_capacity;
		mb->vertex_buffer = (TekMBVertexData*)tek_realloc(mb->vertex_buffer, sizeof(TekMBVertexData) * mb->vertex_buffer_capacity);
	}
	mb->vertex_buffer[mb->vertex_buffer_size++] = vert;
}

void tek_mb_render(TekMeshbuffer* mb)
{
	if (mb->num_commands < 1)
		return;

	//sort

	//lights
	TekDirectionalLight* dlight = nullptr;
	TekPointLight* plights = nullptr;
	if (mb->lights->use_dlight)
	{
		dlight = &mb->lights->dlight;
	}

	if (mb->lights->num_plights > 0)
	{
		plights = mb->lights->plights;
	}

	//vertices
	for (u32 i = 0; i < mb->num_commands; ++i)
	{
		add_vertex(mb, mb->commands[i].vertices[0]);
		add_vertex(mb, mb->commands[i].vertices[1]);
		add_vertex(mb, mb->commands[i].vertices[2]);		
		//tek_renderer_draw_mesh(commands[i].mesh, commands[i].material, &commands[i].transform,cam, dlight, plights,lights->num_plights);
	}

	Mat4 p = mb->cam->projection;
	p = mat4_transposed(&p);

	Mat4 v = mb->cam->view;
	v = mat4_transposed(&v);

	TekShader* shader = tek_renderer_get_meshbuffer_shader();
	tek_shader_bind(shader);

	tek_shader_uniform_mat4(shader, "u_p", &p, 1);
	tek_shader_uniform_mat4(shader, "u_v", &v, 1);

	//set lights
	if (mb->lights->use_dlight)
	{
		tek_shader_uniform_vec3(shader, "u_direct_light.direction", mb->lights->dlight.direction);
		tek_shader_uniform_vec3(shader, "u_direct_light.color", mb->lights->dlight.color);
		tek_shader_uniform_float(shader, "u_direct_light.intensity", mb->lights->dlight.intensity);
		//tek_shader_uniform_vec3(&g_geometry_shader,"u_eye_world_pos", cam->position);
		tek_shader_uniform_int(shader, "u_use_d_light", 1);
	}
	else
	{
		tek_shader_uniform_int(shader, "u_use_d_light", 0);
	}

	if (mb->lights->num_plights > 0)
	{
		tek_shader_uniform_int(shader, "u_use_p_light", 1);
		tek_shader_uniform_int(shader, "u_num_pointlights", mb->lights->num_plights);
		for (u32 i = 0; i < mb->lights->num_plights; ++i)
		{
			char name0[128];
			char name1[128];
			char name2[128];

			sprintf(name0, "u_point_lights[%u].position", i);
			sprintf(name1, "u_point_lights[%u].color", i);
			sprintf(name2, "u_point_lights[%u].range", i);

			tek_shader_uniform_vec3(shader, name0, plights[i].position);
			tek_shader_uniform_vec3(shader, name1, plights[i].color);
			tek_shader_uniform_float(shader, name2, plights[i].range);
		}
	}
	else
	{
		tek_shader_uniform_int(shader, "u_use_p_light", 0);
	}

	for (u32 i = 0; i < mb->num_texture_slots; ++i)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glBindTexture(GL_TEXTURE_2D, mb->texture_slots[i]->id));
	}

	//set materials
	for (u32 i = 0; i < mb->num_material_slots; ++i)
	{
		MaterialCommand cmd = mb->material_slots[i];
		TekMaterial* mat = cmd.material;
		if (mat->has_diffuse_map)
		{
			char name0[64];
			char name1[64];
			sprintf(name0, "u_material_%u.has_diffuse_map", i);
			sprintf(name1, "u_material_%u.diffuse_map", i);
			tek_shader_uniform_int(shader, name0, 1);
			tek_shader_uniform_int(shader, name1, cmd.tex_id);
			tek_tex_bind(&mat->diffuse_map, 0);
		}
		else
		{
			char name0[64];
			char name1[64];
			char name2[64];
			sprintf(name0, "u_material_%u.has_diffuse_map", i);
			sprintf(name1, "u_material_%u.ambient_color", i);
			sprintf(name2, "u_material_%u.diffuse_color", i);

			tek_shader_uniform_int(shader, name0, 0);
			tek_shader_uniform_vec4(shader, name1, tek_color_to_vec4(mat->ambient_color));
			tek_shader_uniform_vec4(shader, name2, tek_color_to_vec4(mat->diffuse_color));
		}
	}

	//drawing
	int size = mb->vertex_buffer_size * sizeof(TekMBVertexData);

	u32 vao = 0;
	u32 vbo = 0;

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, &mb->vertex_buffer[0], GL_STATIC_DRAW));

	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_uv = tek_shader_get_attrib_loc(shader, "a_uv");
	int loc_normal = tek_shader_get_attrib_loc(shader, "a_normal");
	int loc_flags = tek_shader_get_attrib_loc(shader, "a_flags");
	int loc_model = tek_shader_get_attrib_loc(shader, "a_model");


	GLCall(glEnableVertexAttribArray(loc_pos));
	GLCall(glEnableVertexAttribArray(loc_uv));
	GLCall(glEnableVertexAttribArray(loc_normal));
	GLCall(glEnableVertexAttribArray(loc_flags));
	GLCall(glEnableVertexAttribArray(loc_model));
	GLCall(glEnableVertexAttribArray(loc_model+1));
	GLCall(glEnableVertexAttribArray(loc_model+2));
	GLCall(glEnableVertexAttribArray(loc_model+3));

	int stride = sizeof(TekMBVertexData);

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, position))));
	GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, uv))));
	GLCall(glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, normal))));
	GLCall(glVertexAttribPointer(loc_flags, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, flags))));
	GLCall(glVertexAttribPointer(loc_model, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, model0))));
	GLCall(glVertexAttribPointer(loc_model+1, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, model1))));
	GLCall(glVertexAttribPointer(loc_model + 2, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, model2))));
	GLCall(glVertexAttribPointer(loc_model + 3, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMBVertexData, model3))));

	GLCall(glDrawArrays(GL_TRIANGLES,0,mb->vertex_buffer_size));


	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_uv));
	GLCall(glDisableVertexAttribArray(loc_normal));
	GLCall(glDisableVertexAttribArray(loc_flags));
	GLCall(glDisableVertexAttribArray(loc_model));
	GLCall(glDisableVertexAttribArray(loc_model+1));
	GLCall(glDisableVertexAttribArray(loc_model+2));
	GLCall(glDisableVertexAttribArray(loc_model+3));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glDeleteBuffers(1, &vbo));
	GLCall(glDeleteVertexArrays(1, &vao));
}

void tek_mb_reset(TekMeshbuffer* mb)
{
	mb->num_commands = 0;
	mb->num_material_slots = 0;
	mb->num_texture_slots = 0;
	mb->vertex_buffer_size = 0;
}