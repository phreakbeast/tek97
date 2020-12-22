#include "tek_mesh.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_rect.hpp"
#include "tek_shader.hpp"
#include "tek_drawing.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

TekMeshVertexData tek_mesh_vertex_data_create(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
	TekMeshVertexData res;
	res.position = vec3_create(x, y, z);
	res.uv = vec2_create(u, v);
	res.normal = vec3_create(nx, ny, nz);
	return res;
}


void tek_mesh_destroy(TekMesh* mesh)
{
	GLCall(glDeleteBuffers(1, &mesh->vbo));
	GLCall(glDeleteBuffers(1, &mesh->ibo));
	GLCall(glDeleteVertexArrays(1, &mesh->vao));
	tek_free(mesh->vertices);
	tek_free(mesh->indices);
}

void tek_mesh_create(TekMesh* mesh, TekMeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices, bool calc_normals)
{
	mesh->num_vertices = num_vertices;
	mesh->num_indices = num_indices;

	//calculating normals
	if (calc_normals)
	{
		//TODO: check this method. normals may be inverted
		for (u32 i = 0; i < num_indices; i += 3)
		{
			u32 index0 = indices[i];
			u32 index1 = indices[i + 1];
			u32 index2 = indices[i + 2];
			Vec3 v1 = vec3_sub(vertices[index1].position, vertices[index0].position);
			Vec3 v2 = vec3_sub(vertices[index2].position, vertices[index0].position);
			Vec3 normal = vec3_cross(v1, v2);
			normal = vec3_normalized(normal);

			vertices[index0].normal = vec3_add(vertices[index0].normal, normal);
			vertices[index1].normal = vec3_add(vertices[index1].normal, normal);
			vertices[index2].normal = vec3_add(vertices[index2].normal, normal);
		}

		for (u32 i = 0; i < num_vertices; i++)
		{
			vertices[i].normal = vec3_normalized(vertices[i].normal);
		}
	}

	mesh->vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * num_vertices);
	mesh->indices = (u32*)tek_malloc(sizeof(u32) * num_indices);

	memcpy(mesh->vertices, vertices, num_vertices * sizeof(TekMeshVertexData));
	memcpy(mesh->indices, indices, num_indices * sizeof(u32));

	int size = num_vertices * sizeof(TekMeshVertexData);

	GLCall(glGenVertexArrays(1, &mesh->vao));
	GLCall(glBindVertexArray(mesh->vao));

	GLCall(glGenBuffers(1, &mesh->vbo));
	GLCall(glGenBuffers(1, &mesh->ibo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));

	TekShader* shader = tek_renderer_get_geometry_shader();
	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_uv = tek_shader_get_attrib_loc(shader, "a_uv");
	int loc_normal = tek_shader_get_attrib_loc(shader, "a_normal");
	

	GLCall(glEnableVertexAttribArray(loc_pos));
	GLCall(glEnableVertexAttribArray(loc_uv));
	GLCall(glEnableVertexAttribArray(loc_normal));

	int stride = sizeof(TekMeshVertexData);

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMeshVertexData, position))));
	GLCall(glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMeshVertexData, uv))));
	GLCall(glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekMeshVertexData, normal))));


	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(u32), indices, GL_STATIC_DRAW));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_uv));
	GLCall(glDisableVertexAttribArray(loc_normal));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	mesh->min = vec3_create(0, 0, 0);
	mesh->max = vec3_create(0, 0, 0);
	mesh->center = vec3_create(0, 0, 0);

	for (u32 i = 0; i < num_vertices; ++i)
	{
		TekMeshVertexData* v = &vertices[i];
		if (v->position.x < mesh->min.x)
		{
			mesh->min.x = v->position.x;
		}
		if (v->position.x > mesh->max.x)
		{
			mesh->max.x = v->position.x;
		}

		if (v->position.y < mesh->min.y)
		{
			mesh->min.y = v->position.y;
		}
		if (v->position.y > mesh->max.y)
		{
			mesh->max.y = v->position.y;
		}

		if (v->position.z < mesh->min.z)
		{
			mesh->min.z = v->position.z;
		}
		if (v->position.z > mesh->max.z)
		{
			mesh->max.z = v->position.z;
		}
	}

	mesh->center.x = mesh->max.x - mesh->min.x;
	mesh->center.y = mesh->max.y - mesh->min.y;
	mesh->center.z = mesh->max.z - mesh->min.z;
}

void tek_mesh_render(TekMesh* mesh)
{
	TekShader* shader = tek_renderer_get_geometry_shader();
	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_uv = tek_shader_get_attrib_loc(shader, "a_uv");
	int loc_normal = tek_shader_get_attrib_loc(shader, "a_normal");

	GLCall(glBindVertexArray(mesh->vao));

	GLCall(glEnableVertexAttribArray(loc_pos));
	GLCall(glEnableVertexAttribArray(loc_uv));
	GLCall(glEnableVertexAttribArray(loc_normal));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
	GLCall(glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_uv));
	GLCall(glDisableVertexAttribArray(loc_normal));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}


static const char* get_file_extension(const char* filename)
{
	//TODO: implement

	return ".obj";
}

static bool load_obj(TekMesh* mesh, const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error: cannot open file %s\n", filename);
		return false;
	}

	u32 num_position = 0;
	u32 num_normals = 0;
	u32 num_uvs = 0;
	u32 num_faces = 0;

	char line[1024];
	while (fgets(line, 1024, fp))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			num_position++;
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			num_normals++;
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			num_uvs++;
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			num_faces++;
		}
	}

	Vec3* temp_positions = (Vec3*)tek_malloc(sizeof(Vec3) * num_position);
	Vec3* temp_normals = (Vec3*)tek_malloc(sizeof(Vec3) * num_normals);
	Vec3* temp_uvs = (Vec3*)tek_malloc(sizeof(Vec3) * num_uvs);

	TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * num_faces * 3);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * num_faces * 3);

	u32 position_ctr = 0;
	u32 normal_ctr = 0;
	u32 uv_ctr = 0;
	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	fseek(fp, 0, SEEK_SET);
	while (fgets(line, 1024, fp))
	{
		//std::cout << line << std::endl;
		if (line[0] == '\n')
			continue;

		if (line[0] == 'v' && line[1] == ' ')
		{

			float x, y, z;

			int res = sscanf(line, "v %f %f %f", &x, &y, &z);
			if (res < 3)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			Vec3 v = { x, y, z };
			temp_positions[position_ctr++] = v;
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			float x, y, z;

			int res = sscanf(line, "vn %f %f %f", &x, &y, &z);
			if (res < 3)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			Vec3 v = { x, y, z };
			temp_normals[normal_ctr++] = v;
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			float x, y;

			int res = sscanf(line, "vt %f %f", &x, &y);
			if (res < 2)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			Vec3 v = { x, y, 0 };
			temp_uvs[uv_ctr++] = v;
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			int v0, t0, n0;
			int v1, t1, n1;
			int v2, t2, n2;

			int res = sscanf(&line[0], "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
			if (res < 9)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			TekMeshVertexData vert0;
			vert0.position = temp_positions[v0 - 1];
			vert0.normal = temp_normals[n0 - 1];
			vert0.uv.x = temp_uvs[t0 - 1].x;
			vert0.uv.y = temp_uvs[t0 - 1].y;

			vertices[vertex_ctr] = vert0;
			indices[index_ctr++] = vertex_ctr++;

			TekMeshVertexData vert1;
			vert1.position = temp_positions[v1 - 1];
			vert1.normal = temp_normals[n1 - 1];
			vert1.uv.x = temp_uvs[t1 - 1].x;
			vert1.uv.y = temp_uvs[t1 - 1].y;

			vertices[vertex_ctr] = vert1;
			indices[index_ctr++] = vertex_ctr++;

			TekMeshVertexData vert2;
			vert2.position = temp_positions[v2 - 1];
			vert2.normal = temp_normals[n2 - 1];
			vert2.uv.x = temp_uvs[t2 - 1].x;
			vert2.uv.y = temp_uvs[t2 - 1].y;

			vertices[vertex_ctr] = vert2;
			indices[index_ctr++] = vertex_ctr++;
		}
	}

	fclose(fp);

	printf("Mesh file proccessed. Creating geometry ...\n");

	tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, false);

	tek_free(temp_positions);
	tek_free(temp_normals);
	tek_free(temp_uvs);
	tek_free(vertices);
	tek_free(indices);

	return true;
}

static bool load_mesh(TekMesh* mesh, const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error: cannot open file %s\n", filename);
		return false;
	}

	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	char line[1024];
	while (fgets(line, 1024, fp))
	{
		if (line[0] == 'h' && line[1] == 'd')
		{
			u32 num_vertices, num_indices;

			int result = sscanf(line, "hd %u %u", &num_vertices, &num_indices);
			if (result < 2)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			mesh->num_vertices = num_vertices;
			mesh->num_indices = num_indices;

			mesh->indices = (u32*)tek_malloc(sizeof(u32) * num_indices);
			mesh->vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * num_vertices);
		}
		else if (line[0] == 'v' && line[1] == 'x')
		{
			float x, y, z;
			float u, v;
			float nx, ny, nz;

			int result = sscanf(line, "vx %f %f %f %f %f %f %f %f", &x, &y, &z, &u, &v, &nx, &ny, &nz);
			if (result < 8)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			TekMeshVertexData vert = { {x, y, z}, {u, v}, {nx, ny, nz} };
			mesh->vertices[vertex_ctr++] = vert;
		}
		else if (line[0] == 'i' && line[1] == 'd')
		{
			u32 idx;
			int result = sscanf(line, "id %u", &idx);
			if (result < 1)
			{
				printf("Warning: cannot process line: %s File: %s\n", line, filename);
				fclose(fp);
				return false;
			}

			mesh->indices[index_ctr++] = idx;
		}
	}

	fclose(fp);

	tek_mesh_create(mesh, mesh->vertices, vertex_ctr, mesh->indices, index_ctr, false);

	return true;
}

bool tek_mesh_load(TekMesh* mesh, const char* filename)
{
	printf("Loading mesh %s\n", filename);

	bool result = false;
	const char* ext = get_file_extension(filename);
	if (strcmp(ext, ".obj") == 0 || strcmp(ext, ".OBJ") == 0)
	{
		result = load_obj(mesh, filename);
	}
	else if (strcmp(ext, ".mesh") == 0 || strcmp(ext, ".MESH") == 0)
	{
		result = load_mesh(mesh, filename);
	}

	return result;
}


void tek_mesh_create_plane(TekMesh* mesh, float width, float depth, int x_segments, int y_segments)
{
	TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * x_segments * y_segments * 4);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * x_segments * y_segments * 6);

	float x_step = width / x_segments;
	float y_step = depth / y_segments;

	u32 vertex_ctr = 0;
	u32 index_ctr = 0;
	u32 vert_c = 0;
	for (u32 y = 0; y < y_segments; ++y)
	{
		for (u32 x = 0; x < x_segments; ++x)
		{
			float x_pos = x_step * x;
			float y_pos = y_step * y;

			vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x_pos, 0,        y_pos, 0, 1, 0, 1, 0);
			vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x_pos, 0,        y_pos+y_step, 0, 0, 0, 1, 0);
			vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x_pos+x_step, 0, y_pos+y_step, 1, 0, 0, 1, 0);
			vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x_pos+x_step, 0, y_pos, 1, 1, 0, 1, 0);

			indices[index_ctr++] = vert_c + 0;
			indices[index_ctr++] = vert_c + 1;
			indices[index_ctr++] = vert_c + 2;
			indices[index_ctr++] = vert_c + 0;
			indices[index_ctr++] = vert_c + 2;
			indices[index_ctr++] = vert_c + 3;
			vert_c += 4;
		}
	}	

	tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, false);

	tek_free(vertices);
	tek_free(indices);
}

void tek_mesh_create_box(TekMesh* mesh, float width, float height, float depth)
{
	TekMeshVertexData vertices[24];
	u32 indices[36];

	float w = width / 2.0f;
	float d = depth / 2.0f;
	float h = height;

	//front
	vertices[0] = tek_mesh_vertex_data_create(-w, h, d, 0, 1, 0, 0, 1);
	vertices[1] = tek_mesh_vertex_data_create(-w, 0, d, 0, 0, 0, 0, 1);
	vertices[2] = tek_mesh_vertex_data_create(w, 0, d, 1, 0, 0, 0, 1);
	vertices[3] = tek_mesh_vertex_data_create(w, h, d, 1, 1, 0, 0, 1);

	//left
	vertices[4] = tek_mesh_vertex_data_create(-w, h, -d, 0, 1, -1, 0, 0);
	vertices[5] = tek_mesh_vertex_data_create(-w, 0, -d, 0, 0, -1, 0, 0);
	vertices[6] = tek_mesh_vertex_data_create(-w, 0, d, 1, 0, -1, 0, 0);
	vertices[7] = tek_mesh_vertex_data_create(-w, h, d, 1, 1, -1, 0, 0);

	//right
	vertices[8] = tek_mesh_vertex_data_create(w, h, d, 0, 1, 1, 0, 0);
	vertices[9] = tek_mesh_vertex_data_create(w, 0, d, 0, 0, 1, 0, 0);
	vertices[10] = tek_mesh_vertex_data_create(w, 0, -d, 1, 0, 1, 0, 0);
	vertices[11] = tek_mesh_vertex_data_create(w, h, -d, 1, 1, 1, 0, 0);

	//back
	vertices[12] = tek_mesh_vertex_data_create(w, h, -d, 0, 1, 0, 0, -1);
	vertices[13] = tek_mesh_vertex_data_create(w, 0, -d, 0, 0, 0, 0, -1);
	vertices[14] = tek_mesh_vertex_data_create(-w, 0, -d, 1, 0, 0, 0, -1);
	vertices[15] = tek_mesh_vertex_data_create(-w, h, -d, 1, 1, 0, 0, -1);

	//top
	vertices[16] = tek_mesh_vertex_data_create(-w, h, -d, 0, 1, 0, 1, 0);
	vertices[17] = tek_mesh_vertex_data_create(-w, h, d, 0, 0, 0, 1, 0);
	vertices[18] = tek_mesh_vertex_data_create(w, h, d, 1, 0, 0, 1, 0);
	vertices[19] = tek_mesh_vertex_data_create(w, h, -d, 1, 1, 0, 1, 0);

	//bottom
	vertices[20] = tek_mesh_vertex_data_create(-w, 0, d, 0, 1, 0, -1, 0);
	vertices[21] = tek_mesh_vertex_data_create(-w, 0, -d, 0, 0, 0, -1, 0);
	vertices[22] = tek_mesh_vertex_data_create(w, 0, -d, 1, 0, 0, -1, 0);
	vertices[23] = tek_mesh_vertex_data_create(w, 0, d, 1, 1, 0, -1, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 4;
	indices[10] = 6;
	indices[11] = 7;

	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;
	indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;

	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;

	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;

	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;

	tek_mesh_create(mesh, vertices, 24, indices, 36, false);
}

void tek_mesh_create_skybox(TekMesh* mesh, float width)
{
	TekMeshVertexData vertices[24];
	u32 indices[36];

	float w = width / 2.0f;
	float d = width / 2.0f;
	float h = width / 2.0f;

	TekRect uv = TekRect(0, 0, 256, 256, 256 * 3, 256 * 3);

	//front
	vertices[0] = tek_mesh_vertex_data_create(w, h, d, uv.x, uv.h, 0, 0, -1);
	vertices[1] = tek_mesh_vertex_data_create(w, -h, d, uv.x, uv.y, 0, 0, -1);
	vertices[2] = tek_mesh_vertex_data_create(-w, -h, d, uv.w, uv.y, 0, 0, -1);
	vertices[3] = tek_mesh_vertex_data_create(-w, h, d, uv.w, uv.h, 0, 0, -1);

	//left
	uv = TekRect(256, 0, 256, 256, 256 * 3, 256 * 3);
	vertices[4] = tek_mesh_vertex_data_create(-w, h, d, uv.x, uv.h, 1, 0, 0);
	vertices[5] = tek_mesh_vertex_data_create(-w, -h, d, uv.x, uv.y, 1, 0, 0);
	vertices[6] = tek_mesh_vertex_data_create(-w, -h, -d, uv.w, uv.y, 1, 0, 0);
	vertices[7] = tek_mesh_vertex_data_create(-w, h, -d, uv.w, uv.h, 1, 0, 0);

	//right
	uv = TekRect(512, 0, 256, 256, 256 * 3, 256 * 3);
	vertices[8] = tek_mesh_vertex_data_create(w, h, -d, uv.x, uv.h, -1, 0, 0);
	vertices[9] = tek_mesh_vertex_data_create(w, -h, -d, uv.x, uv.y, -1, 0, 0);
	vertices[10] = tek_mesh_vertex_data_create(w, -h, d, uv.w, uv.y, -1, 0, 0);
	vertices[11] = tek_mesh_vertex_data_create(w, h, d, uv.w, uv.h, -1, 0, 0);

	//back
	uv = TekRect(0, 256, 256, 256, 256 * 3, 256 * 3);
	vertices[12] = tek_mesh_vertex_data_create(-w, h, -d, uv.x, uv.h, 0, 0, 1);
	vertices[13] = tek_mesh_vertex_data_create(-w, -h, -d, uv.x, uv.y, 0, 0, 1);
	vertices[14] = tek_mesh_vertex_data_create(w, -h, -d, uv.w, uv.y, 0, 0, 1);
	vertices[15] = tek_mesh_vertex_data_create(w, h, -d, uv.w, uv.h, 0, 0, 1);

	//top
	uv = TekRect(256, 256, 256, 256, 256 * 3, 256 * 3);
	vertices[16] = tek_mesh_vertex_data_create(-w, h, d, uv.x, uv.h, 0, -1, 0);
	vertices[17] = tek_mesh_vertex_data_create(-w, h, -d, uv.x, uv.y, 0, -1, 0);
	vertices[18] = tek_mesh_vertex_data_create(w, h, -d, uv.w, uv.y, 0, -1, 0);
	vertices[19] = tek_mesh_vertex_data_create(w, h, d, uv.w, uv.h, 0, -1, 0);

	//bottom
	uv = TekRect(512, 256, 256, 256, 256 * 3, 256 * 3);
	vertices[20] = tek_mesh_vertex_data_create(-w, -h, -d, uv.x, uv.h, 0, 1, 0);
	vertices[21] = tek_mesh_vertex_data_create(-w, -h, d, uv.x, uv.y, 0, 1, 0);
	vertices[22] = tek_mesh_vertex_data_create(w, -h, d, uv.w, uv.y, 0, 1, 0);
	vertices[23] = tek_mesh_vertex_data_create(w, -h, -d, uv.w, uv.h, 0, 1, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 4;
	indices[10] = 6;
	indices[11] = 7;

	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;
	indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;

	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;

	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;

	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;

	tek_mesh_create(mesh, vertices, 24, indices, 36, false);
}

void tek_mesh_create_cylinder(TekMesh* mesh, float rad, int segment, float h, float uv_x, float uv_y)
{
	TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * segment * 6);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * segment * 12);

	Vec2* points = (Vec2*)tek_malloc(sizeof(Vec2) * segment);

	u32 point_ctr = 0;
	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	float seg = 360.0f / segment;
	for (int i = 0; i < segment; ++i)
	{
		float x, y;
		float angle = seg * i;

		math_get_circle_coord(rad, 0, 0, angle, &x, &y);
		Vec2 v = { x, y };
		points[point_ctr++] = v;
	}

	for (int i = 0; i < segment; ++i)
	{
		float x1 = points[i].x;
		float y1 = points[i].y;

		float x2, y2;
		if (i == segment - 1)
		{
			x2 = points[0].x;
			y2 = points[0].y;
		}
		else
		{
			x2 = points[i + 1].x;
			y2 = points[i + 1].y;
		}

		float fixed_uv_x1 = ((float)i / segment) * uv_x;
		float fixed_uv_x2;

		if (i == segment - 1)
		{
			fixed_uv_x2 = 1 * uv_x;
		}
		else
		{
			fixed_uv_x2 = ((float)(i + 1) / segment) * uv_x;
		}

		u32 vert_c = vertex_ctr;

		Vec3 n1 = { x1, 0, y1 };
		Vec3 n2 = { x2, 0, y2 };
		n1 = vec3_normalized(n1);
		n2 = vec3_normalized(n2);

		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x2, h, y2, fixed_uv_x2, 0, n2.x, n2.y, n2.z); //0
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x2, 0, y2, fixed_uv_x2, uv_y, n2.x, n2.y, n2.z); //1

		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x1, 0, y1, fixed_uv_x1, uv_y, n1.x, n1.y, n1.z); //2
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x1, h, y1, fixed_uv_x1, 0, n1.x, n1.y, n1.z); //3

		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(0, h, 0, 0, 0, 0, 1, 0); //4
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(0, 0, 0, 0, 0, 0, -1, 0); //5

		indices[index_ctr++] = (vert_c + 0);
		indices[index_ctr++] = (vert_c + 1);
		indices[index_ctr++] = (vert_c + 2);

		indices[index_ctr++] = (vert_c + 0);
		indices[index_ctr++] = (vert_c + 2);
		indices[index_ctr++] = (vert_c + 3);

		indices[index_ctr++] = (vert_c + 4);
		indices[index_ctr++] = (vert_c + 0);
		indices[index_ctr++] = (vert_c + 3);

		indices[index_ctr++] = (vert_c + 1);
		indices[index_ctr++] = (vert_c + 5);
		indices[index_ctr++] = (vert_c + 2);
	}

	tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, 1);

	tek_free(points);
	tek_free(vertices);
	tek_free(indices);
}

void tek_mesh_create_inner_cylinder(TekMesh* mesh, float rad, int segment, float h, float uv_x, float uv_y, int caps)
{
	TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * segment * 6);
	u32* indices = NULL;
	if (caps)
	{
		indices = (u32*)tek_malloc(sizeof(u32) * segment * 12);
	}
	else
	{
		indices = (u32*)tek_malloc(sizeof(u32) * segment * 6);
	}
	assert(caps);

	Vec2* points = (Vec2*)tek_malloc(sizeof(Vec2) * segment);

	u32 point_ctr = 0;
	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	float seg = 360.0f / segment;
	for (int i = 0; i < segment; ++i)
	{
		float x, y;
		float angle = seg * i;

		math_get_circle_coord(rad, 0, 0, angle, &x, &y);
		Vec2 v = { x, y };
		points[point_ctr++] = v;
	}

	for (int i = 0; i < segment; ++i)
	{
		float x1 = points[i].x;
		float y1 = points[i].y;

		float x2, y2;
		if (i == segment - 1)
		{
			x2 = points[0].x;
			y2 = points[0].y;
		}
		else
		{
			x2 = points[i + 1].x;
			y2 = points[i + 1].y;
		}

		float fixed_uv_x1 = ((float)i / segment) * uv_x;
		float fixed_uv_x2;

		if (i == segment - 1)
		{
			fixed_uv_x2 = 1 * uv_x;
		}
		else
		{
			fixed_uv_x2 = ((float)(i + 1) / segment) * uv_x;
		}

		u32 vert_c = vertex_ctr;;

		Vec3 n1 = vec3_create(x1, 0, y1);
		Vec3 n2 = vec3_create(x2, 0, y2);
		n1 = vec3_normalized(n1);
		n2 = vec3_normalized(n2);
		n1 = vec3_negate(n1);
		n2 = vec3_negate(n2);

		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x2, h, y2, fixed_uv_x2, 0, n2.x, n2.y, n2.z); //0
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x2, 0, y2, fixed_uv_x2, uv_y, n2.x, n2.y, n2.z); //1
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x1, 0, y1, fixed_uv_x1, uv_y, n1.x, n1.y, n1.z); //2
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(x1, h, y1, fixed_uv_x1, 0, n1.x, n1.y, n1.z); //3
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(0, h, 0, 0, 0, 0, 1, 0); //4
		vertices[vertex_ctr++] = tek_mesh_vertex_data_create(0, 0, 0, 0, 0, 0, -1, 0); //5


		indices[index_ctr++] = (vert_c + 0);
		indices[index_ctr++] = (vert_c + 2);
		indices[index_ctr++] = (vert_c + 1);
		indices[index_ctr++] = (vert_c + 0);
		indices[index_ctr++] = (vert_c + 3);
		indices[index_ctr++] = (vert_c + 2);


		if (caps)
		{
			indices[index_ctr++] = (vert_c + 4);
			indices[index_ctr++] = (vert_c + 0);
			indices[index_ctr++] = (vert_c + 3);
			indices[index_ctr++] = (vert_c + 1);
			indices[index_ctr++] = (vert_c + 5);
			indices[index_ctr++] = (vert_c + 2);
		}
	}

	tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, 1);

	tek_free(points);
	tek_free(vertices);
	tek_free(indices);
}

void tek_mesh_create_sphere(TekMesh* mesh, float rad, int rings, int sectors, int inverted)
{
	TekMeshVertexData* vertices = (TekMeshVertexData*)tek_malloc(sizeof(TekMeshVertexData) * rings * sectors);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * rings * sectors * 6);

	u32 vertex_ctr = 0;
	u32 index_ctr = 0;

	const float R = 1.0f / (float)(rings - 1);
	const float S = 1.0f / (float)(sectors - 1);

	for (int r = 0; r < rings; r++)
	{
		for (int s = 0; s < sectors; s++)
		{
			float const y = sinf(-M_PI_2 + M_PI * r * R);
			float const x = cosf(2 * M_PI * s * S) * sinf(M_PI * r * R);
			float const z = sinf(2 * M_PI * s * S) * sinf(M_PI * r * R);


			float u = s * S;
			float v = r * R;

			float px = x * rad;
			float py = y * rad;
			float pz = z * rad;

			float nx = x;
			float ny = y;
			float nz = z;

			vertices[vertex_ctr++] = tek_mesh_vertex_data_create(px, py, pz, u, v, nx, ny, nz);

		}
	}

	for (int r = 0; r < rings; r++)
	{
		for (int s = 0; s < sectors; s++)
		{
			if (inverted)
			{
				indices[index_ctr++] = (r * sectors + s); //0
				indices[index_ctr++] = (r * sectors + (s + 1)); //1
				indices[index_ctr++] = ((r + 1) * sectors + (s + 1)); //2
				indices[index_ctr++] = (r * sectors + s); //0
				indices[index_ctr++] = ((r + 1) * sectors + (s + 1)); //2
				indices[index_ctr++] = ((r + 1) * sectors + s); //3
			}
			else
			{
				indices[index_ctr++] = (r * sectors + s); //0
				indices[index_ctr++] = ((r + 1) * sectors + (s + 1)); //2
				indices[index_ctr++] = (r * sectors + (s + 1)); //1
				indices[index_ctr++] = (r * sectors + s); //0
				indices[index_ctr++] = ((r + 1) * sectors + s); //3
				indices[index_ctr++] = ((r + 1) * sectors + (s + 1)); //2
			}
		}
	}

	tek_mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, true);

	tek_free(vertices);
	tek_free(indices);
}

