#include "tek_meshbuffer.hpp"

#include "tek_drawing.hpp"
#include <string.h>

TekMeshbuffer::TekMeshbuffer(TekCamera* cam, TekLightManager* lights, OrderType order)
{
	this->cam = cam;
	this->lights = lights;
}

TekMeshbuffer::~TekMeshbuffer()
{

}

Vec2 TekMeshbuffer::add_material(TekMaterial* material)
{
	//check if texture exists
	u32 tex_id = 0;
	if (material->has_diffuse_map)
	{
		bool found = false;
		for (u32 i = 0; i < texture_slots.size(); ++i)
		{
			if (texture_slots[i]->id == material->diffuse_map.id)
			{
				found = true;
				tex_id = i;
				break;
			}
		}
		if (!found)
		{
			//check if there is room for a texture map
			if (texture_slots.size() >= 8)
			{
				render();
				reset();
			}
			texture_slots.push_back(&material->diffuse_map);
			tex_id = texture_slots.size() - 1;
		}
	}


	float mat_id = 0;
	//check if material exist
	bool found = false;
	for (u32 i = 0; i < material_slots.size(); ++i)
	{
		if (material_slots[i].material == material)
		{
			mat_id = i;
			found = true;
			break;
		}
	}
	if (!found)
	{
		//check if there is room for a material
		if (material_slots.size() >= 8)
		{
			render();
			reset();
		}
		MaterialCommand cmd;
		cmd.material = material;
		cmd.tex_id = tex_id;
		material_slots.push_back(cmd);
		mat_id = material_slots.size() - 1;
	}

	return vec2_create(mat_id, tex_id);
}

void TekMeshbuffer::draw_mesh(TekMesh* mesh, TekMaterial* material, TekTransform* transform)
{
	//early check draw distance

	Mat4 trans = transform->matrix;
	trans = mat4_transposed(&trans);

	Vec2 ids = add_material(material);
	float mat_id = ids.x;
	float tex_id = ids.y;
	
	for (u32 i = 0; i < mesh->num_indices; i += 3)
	{
		TekMeshVertexData v0 = mesh->vertices[mesh->indices[i + 0]];
		TekMeshVertexData v1 = mesh->vertices[mesh->indices[i + 1]];
		TekMeshVertexData v2 = mesh->vertices[mesh->indices[i + 2]];

		DrawCommand dc;
		VertexData vert0;
		VertexData vert1;
		VertexData vert2;
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

		dc.distance = vec3_distance(cam->get_position(), transform->position);

		commands.push_back(dc);
	}	
	
}

void TekMeshbuffer::draw_vertices(TekMeshVertexData* vertices, u32 num_vertices, TekMaterial* material)
{
	Mat4 trans = Mat4().transposed();

	Vec2 ids = add_material(material);
	float mat_id = ids.x;
	float tex_id = ids.y;

	for (u32 i = 0; i < num_vertices; i += 3)
	{
		TekMeshVertexData v0 = vertices[i + 0];
		TekMeshVertexData v1 = vertices[i + 1];
		TekMeshVertexData v2 = vertices[i + 2];

		DrawCommand dc;
		VertexData vert0;
		VertexData vert1;
		VertexData vert2;
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

		dc.distance = vec3_distance(cam->get_position(), vert0.position);

		commands.push_back(dc);
	}
}

void TekMeshbuffer::draw_billboard(TekBillboard* bb, TekMaterial* material, TekTransform* transform, bool spherical)
{
	Mat4 trans = transform->matrix;
	trans = mat4_transposed(&trans);

	Vec2 ids = add_material(material);
	float mat_id = ids.x;
	float tex_id = ids.y;

	for (u32 i = 0; i < 6; i += 3)
	{
		TekMeshVertexData v0 = bb->vertices[i + 0];
		TekMeshVertexData v1 = bb->vertices[i + 1];
		TekMeshVertexData v2 = bb->vertices[i + 2];

		DrawCommand dc;
		VertexData vert0;
		VertexData vert1;
		VertexData vert2;
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

		dc.distance = vec3_distance(cam->get_position(), transform->position);

		commands.push_back(dc);
	}
}

void TekMeshbuffer::render()
{
	//sort

	//lights
	TekDirectionalLight* dlight = nullptr;
	TekPointLight* plights = nullptr;
	if (lights->use_dlight)
	{
		dlight = &lights->dlight;
	}

	if (lights->num_plights > 0)
	{
		plights = lights->plights;
	}

	//vertices
	std::vector<VertexData> vertices;
	for (u32 i = 0; i < commands.size(); ++i)
	{
		vertices.push_back(commands[i].vertices[0]);
		vertices.push_back(commands[i].vertices[1]);
		vertices.push_back(commands[i].vertices[2]);
		//tek_renderer_draw_mesh(commands[i].mesh, commands[i].material, &commands[i].transform,cam, dlight, plights,lights->num_plights);
	}

	Mat4 p = cam->get_projection();
	p = mat4_transposed(&p);

	Mat4 v = cam->get_view();
	v = mat4_transposed(&v);

	TekShader* shader = tek_renderer_get_meshbuffer_shader();
	tek_shader_bind(shader);

	tek_shader_uniform_mat4(shader, "u_p", &p, 1);
	tek_shader_uniform_mat4(shader, "u_v", &v, 1);

	//set lights
	if (lights->use_dlight)
	{
		tek_shader_uniform_vec3(shader, "u_direct_light.direction", lights->dlight.direction);
		tek_shader_uniform_vec3(shader, "u_direct_light.color", lights->dlight.color);
		tek_shader_uniform_float(shader, "u_direct_light.intensity", lights->dlight.intensity);
		//tek_shader_uniform_vec3(&g_geometry_shader,"u_eye_world_pos", cam->position);
		tek_shader_uniform_int(shader, "u_use_d_light", 1);
	}
	else
	{
		tek_shader_uniform_int(shader, "u_use_d_light", 0);
	}

	if (lights->num_plights > 0)
	{
		tek_shader_uniform_int(shader, "u_use_p_light", 1);
		tek_shader_uniform_int(shader, "u_num_pointlights", lights->num_plights);
		for (u32 i = 0; i < lights->num_plights; ++i)
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

	for (u32 i = 0; i < texture_slots.size(); ++i)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture_slots[i]->id));
	}

	//set materials
	for (u32 i = 0; i < material_slots.size(); ++i)
	{
		MaterialCommand cmd = material_slots[i];
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
			tek_shader_uniform_vec4(shader, name1, mat->ambient_color.to_vec4());
			tek_shader_uniform_vec4(shader, name2, mat->diffuse_color.to_vec4());
		}
	}

	//drawing
	int size = vertices.size() * sizeof(VertexData);

	u32 vao = 0;
	u32 vbo = 0;

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW));

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

	int stride = sizeof(VertexData);

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, position))));
	GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, uv))));
	GLCall(glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, normal))));
	GLCall(glVertexAttribPointer(loc_flags, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, flags))));
	GLCall(glVertexAttribPointer(loc_model, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, model0))));
	GLCall(glVertexAttribPointer(loc_model+1, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, model1))));
	GLCall(glVertexAttribPointer(loc_model + 2, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, model2))));
	GLCall(glVertexAttribPointer(loc_model + 3, 4, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, model3))));

	GLCall(glDrawArrays(GL_TRIANGLES,0,vertices.size()));


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



void TekMeshbuffer::reset()
{
	commands.clear();
	material_slots = std::vector<MaterialCommand>();
	texture_slots = std::vector<TekTexture*>();
}