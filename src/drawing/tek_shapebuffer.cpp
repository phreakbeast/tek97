#include "tek_shapebuffer.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_drawing.hpp"

void tek_shapebuffer_init(TekShapebuffer* buffer, TekCamera* cam)
{
	buffer->cam = cam;
	buffer->buffer_size = 0;
	buffer->buffer_capacity = 100;
	buffer->buffer = (TekSBufferVertexData*)tek_malloc(sizeof(TekSBufferVertexData) * buffer->buffer_capacity);
}

void tek_shapebuffer_destroy(TekShapebuffer* buffer)
{
	tek_free(buffer->buffer);
}

static void add_vertex(TekShapebuffer* buffer, TekSBufferVertexData vert)
{
	if (buffer->buffer_size >= buffer->buffer_capacity)
	{
		buffer->buffer_capacity += buffer->buffer_capacity;
		buffer->buffer = (TekSBufferVertexData*)tek_realloc(buffer->buffer, sizeof(TekSBufferVertexData) * buffer->buffer_capacity);
	}

	buffer->buffer[buffer->buffer_size++] = vert;
}

void tek_shapebuffer_draw_line(TekShapebuffer* buffer, Vec3 p0, Vec3 p1, TekColor color)
{
	TekSBufferVertexData v1;
	TekSBufferVertexData v0;

	v0.position = p0;
	v0.color = tek_color_to_vec3(color);

	v1.position = p1;
	v1.color = tek_color_to_vec3(color);
	
	add_vertex(buffer, v0);
	add_vertex(buffer, v1);
}

void tek_shapebuffer_render(TekShapebuffer* buffer)
{
	if (buffer->buffer_size < 1)
		return;

	TekShader* shader = tek_renderer_get_shape_shader();
	tek_shader_bind(shader);

	Mat4 mvp = mat4_mul2(&buffer->cam->projection, &buffer->cam->view);
	mvp = mat4_transposed(&mvp);

	tek_shader_uniform_mat4(shader, "u_mvp", &mvp, 1);

	u32 vao;
	u32 vbo;

	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_color = tek_shader_get_attrib_loc(shader, "a_color");

	int size = buffer->buffer_size * sizeof(TekSBufferVertexData);

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, &buffer->buffer[0], GL_STATIC_DRAW));

	int stride = sizeof(TekSBufferVertexData);

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(TekSBufferVertexData, position))));
	GLCall(glEnableVertexAttribArray(loc_pos));

	GLCall(glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(TekSBufferVertexData, color))));
	GLCall(glEnableVertexAttribArray(loc_color));	


	GLCall(glDrawArrays(GL_LINES, 0, buffer->buffer_size));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_color));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glDeleteBuffers(1, &vbo));
	GLCall(glDeleteVertexArrays(1, &vao));
}

void tek_shapebuffer_reset(TekShapebuffer* buffer)
{
	buffer->buffer_size = 0;
}