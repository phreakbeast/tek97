#include "tek_shapebuffer.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_drawing.hpp"

void tek_shapebuffer_init(TekShapebuffer* buffer, TekCamera* cam)
{
	buffer->cam = cam;
}

void tek_shapebuffer_destroy(TekShapebuffer* buffer)
{

}

void tek_shapebuffer_draw_line(TekShapebuffer* buffer, Vec3 p0, Vec3 p1, TekColor color)
{
	TekSBufferVertexData v1;
	TekSBufferVertexData v0;

	v0.position = p0;
	v0.color = color.to_vec3();

	v1.position = p1;
	v1.color = color.to_vec3();

	buffer->buffer.push_back(v0);
	buffer->buffer.push_back(v1);
}

void tek_shapebuffer_render(TekShapebuffer* buffer)
{
	TekShader* shader = tek_renderer_get_shape_shader();
	tek_shader_bind(shader);

	Mat4 mvp = buffer->cam->get_projection() * buffer->cam->get_view();
	mvp = mat4_transposed(&mvp);

	tek_shader_uniform_mat4(shader, "u_mvp", &mvp, 1);

	u32 vao;
	u32 vbo;

	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_color = tek_shader_get_attrib_loc(shader, "a_color");

	int size = buffer->buffer.size() * sizeof(TekSBufferVertexData);

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


	GLCall(glDrawArrays(GL_LINES, 0, buffer->buffer.size()));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_color));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glDeleteBuffers(1, &vbo));
	GLCall(glDeleteVertexArrays(1, &vao));
}

void tek_shapebuffer_reset(TekShapebuffer* buffer)
{
	buffer->buffer.clear();
}