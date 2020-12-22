#include "tek_shape.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_shader.hpp"
#include "tek_drawing.hpp"
#include <stdlib.h>


void tek_shape_destroy(TekShape* shape)
{
	GLCall(glDeleteBuffers(1, &shape->vbo));
	GLCall(glDeleteBuffers(1, &shape->ibo));
	GLCall(glDeleteVertexArrays(1, &shape->vao));
}

void tek_shape_create(TekShape* shape, TekShapeVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
	shape->num_vertices = num_vertices;
	shape->num_indices = num_indices;

	int size = num_vertices * sizeof(TekShapeVertexData);

	GLCall(glGenVertexArrays(1, &shape->vao));
	GLCall(glBindVertexArray(shape->vao));

	GLCall(glGenBuffers(1, &shape->vbo));
	GLCall(glGenBuffers(1, &shape->ibo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, shape->vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));

	int stride = sizeof(TekShapeVertexData);

	TekShader* shader = tek_renderer_get_shape_shader();
	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_color = tek_shader_get_attrib_loc(shader, "a_color");

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(TekShapeVertexData, position))));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekShapeVertexData, color))));
	GLCall(glEnableVertexAttribArray(1));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(u32), indices, GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void tek_shape_render(TekShape* shape)
{
	GLCall(glBindVertexArray(shape->vao));

	TekShader* shader = tek_renderer_get_shape_shader();
	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_color = tek_shader_get_attrib_loc(shader, "a_color");

	GLCall(glEnableVertexAttribArray(loc_pos));
	GLCall(glEnableVertexAttribArray(loc_color));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ibo));
	GLCall(glDrawElements(GL_LINES, shape->num_indices, GL_UNSIGNED_INT, 0));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_color));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void tek_shape_create_line(TekShape* shape, Vec3 p0, Vec3 p1, TekColor color)
{
	TekShapeVertexData vertices[2];
	u32 indices[2];

	TekShapeVertexData v0 = { p0, color.to_vec3() };
	TekShapeVertexData v1 = { p1, color.to_vec3() };

	vertices[0] = v0;
	vertices[1] = v1;

	indices[0] = 0;
	indices[1] = 1;

	tek_shape_create(shape, vertices, 2, indices, 2);
}

void tek_shape_create_box(TekShape* shape, Vec3 min, Vec3 max, TekColor color)
{
	TekShapeVertexData vertices[8];
	u32 indices[24];

	TekShapeVertexData v0 = { vec3_create(min.x, max.y, min.z), color.to_vec3() };
	TekShapeVertexData v1 = { vec3_create(min.x, max.y, max.z), color.to_vec3() };
	TekShapeVertexData v2 = { vec3_create(min.x, min.y, max.z), color.to_vec3() };
	TekShapeVertexData v3 = { vec3_create(min.x, min.y, min.z), color.to_vec3() };
	TekShapeVertexData v4 = { vec3_create(max.x, max.y, min.z), color.to_vec3() };
	TekShapeVertexData v5 = { vec3_create(max.x, max.y, max.z), color.to_vec3() };
	TekShapeVertexData v6 = { vec3_create(max.x, min.y, max.z), color.to_vec3() };
	TekShapeVertexData v7 = { vec3_create(max.x, min.y, min.z), color.to_vec3() };

	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	vertices[3] = v3;
	vertices[4] = v4;
	vertices[5] = v5;
	vertices[6] = v6;
	vertices[7] = v7;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 2;
	indices[5] = 3;
	indices[6] = 3;
	indices[7] = 0;

	indices[8] = 4;
	indices[9] = 5;
	indices[10] = 5;
	indices[11] = 6;
	indices[12] = 6;
	indices[13] = 7;
	indices[14] = 7;
	indices[15] = 4;

	indices[16] = 0;
	indices[17] = 4;
	indices[18] = 1;
	indices[19] = 5;
	indices[20] = 2;
	indices[21] = 6;
	indices[22] = 3;
	indices[23] = 7;

	tek_shape_create(shape, vertices, 8, indices, 24);
}

void
tek_shape_create_circle(TekShape* shape, Vec3 center, float radius, TekColor color, u32 segments, float start_angle,
	float end_angle)
{
	Vec3* points = (Vec3*)tek_malloc(sizeof(Vec3) * segments);
	float step = 0;
	if (end_angle > start_angle)
	{
		step = (end_angle - start_angle) / segments;
	}
	else
	{
		step = ((360 - start_angle) + end_angle) / segments;
	}
	for (u32 i = 0; i < segments + 1; ++i)
	{
		float angle = start_angle + i * step;
		float x;
		float y;
		math_get_circle_coord(radius, center.x, center.y, angle, &x, &y);
		points[i] = vec3_create(x, y, 0);
	}

	TekShapeVertexData* vertices = (TekShapeVertexData*)tek_malloc(sizeof(TekShapeVertexData) * segments);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * segments * 2);

	Vec3 color_vec = color.to_vec3();
	u32 index_ctr = 0;

	for (u32 i = 0; i < segments; ++i)
	{
		TekShapeVertexData v0 = { points[i], color_vec };
		vertices[i] = v0;
		indices[index_ctr++] = i;
		indices[index_ctr++] = i + 1;
	}

	tek_shape_create(shape, vertices, segments, indices, segments * 2);

	tek_free(points);
	tek_free(vertices);
	tek_free(indices);
}

void tek_shape_create_bone(TekShape* shape, float width, float height, TekColor color)
{
	TekShapeVertexData vertices[6];
	u32 indices[24];

	float w = width / 2;
	float h = height;
	float hh = height / 2;

	TekShapeVertexData v0 = { vec3_create(0, h, 0), color.to_vec3() };
	TekShapeVertexData v1 = { vec3_create(-w, hh, -w), color.to_vec3() };
	TekShapeVertexData v2 = { vec3_create(-w, hh, w), color.to_vec3() };
	TekShapeVertexData v3 = { vec3_create(w, hh, w), color.to_vec3() };
	TekShapeVertexData v4 = { vec3_create(w, hh, -w), color.to_vec3() };
	TekShapeVertexData v5 = { vec3_create(0, 0, 0), color.to_vec3() };

	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	vertices[3] = v3;
	vertices[4] = v4;
	vertices[5] = v5;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 2;
	indices[4] = 0;
	indices[5] = 3;
	indices[6] = 0;
	indices[7] = 4;

	indices[8] = 5;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 2;
	indices[12] = 5;
	indices[13] = 3;
	indices[14] = 5;
	indices[15] = 4;

	indices[16] = 1;
	indices[17] = 2;
	indices[18] = 2;
	indices[19] = 3;
	indices[20] = 3;
	indices[21] = 4;
	indices[22] = 4;
	indices[23] = 1;

	tek_shape_create(shape, vertices, 6, indices, 24);
}

void tek_shape_create_grid(TekShape* shape, float width, float height, u32 segs_x, u32 segs_y, TekColor color)
{
	TekShapeVertexData* vertices = (TekShapeVertexData*)tek_malloc(sizeof(TekShapeVertexData) * segs_x * 2 * segs_y * 2);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * segs_x * 2 * segs_y * 2);

	float x_step = width / segs_x;
	float y_step = height / segs_y;

	u32 vertex_ctr = 0;
	u32 index_ctr = 0;
	for (u32 i = 0; i <= segs_x; ++i)
	{
		float pos = i * x_step;

		TekShapeVertexData v0 = { vec3_create(pos, 0, 0), color.to_vec3() };
		TekShapeVertexData v1 = { vec3_create(pos, 0, height), color.to_vec3() };

		vertices[vertex_ctr++] = v0;
		vertices[vertex_ctr++] = v1;
	}

	for (u32 i = 0; i <= segs_y; ++i)
	{
		float pos = i * y_step;

		TekShapeVertexData v0 = { vec3_create(0, 0, pos), color.to_vec3() };
		TekShapeVertexData v1 = { vec3_create(width, 0, pos), color.to_vec3() };

		vertices[vertex_ctr++] = v0;
		vertices[vertex_ctr++] = v1;
	}

	for (u32 i = 0; i < segs_x * 2 * segs_y * 2; ++i)
	{
		indices[index_ctr++] = i;
	}

	tek_shape_create(shape, vertices, vertex_ctr, indices, index_ctr);

	tek_free(vertices);
	tek_free(indices);
}

void tek_shape_create_normals_for_mesh(TekShape* shape, TekMesh* mesh, TekColor color, float length)
{
	u32 num_normals = mesh->num_vertices;

	TekShapeVertexData* vertices = (TekShapeVertexData*)tek_malloc(sizeof(TekShapeVertexData) * num_normals * 2);
	u32* indices = (u32*)tek_malloc(sizeof(u32) * num_normals * 2);

	u32 index_ctr = 0;
	u32 vertex_ctr = 0;
	for (u32 i = 0; i < num_normals; ++i)
	{
		Vec3 p0 = mesh->vertices[i].position;
		Vec3 p1 = vec3_add(p0, vec3_mul(mesh->vertices[i].normal, length));

		TekShapeVertexData vert0;
		vert0.position = p0;
		vert0.color = color.to_vec3();

		TekShapeVertexData vert1;
		vert1.position = p1;
		vert1.color = color.to_vec3();

		vertices[vertex_ctr++] = vert0;
		vertices[vertex_ctr++] = vert1;

		indices[index_ctr] = index_ctr;
		index_ctr++;
		indices[index_ctr] = index_ctr;
		index_ctr++;
	}

	tek_shape_create(shape, vertices, vertex_ctr, indices, index_ctr);
}

