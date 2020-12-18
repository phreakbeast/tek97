#include "tek_shapebuffer.hpp"

#include "../platform/tek_platform.hpp"
#include "tek_drawing.hpp"

TekShapebuffer::TekShapebuffer(TekCamera* cam)
{
	this->cam = cam;
}

TekShapebuffer::~TekShapebuffer()
{

}

void TekShapebuffer::draw_line(Vec3 p0, Vec3 p1, TekColor color)
{
	VertexData v0;
	VertexData v1;

	v0.position = p0;
	v0.color = tek_color_to_vec3(color);

	v1.position = p1;
	v1.color = tek_color_to_vec3(color);

	buffer.push_back(v0);
	buffer.push_back(v1);
}

void TekShapebuffer::render()
{
	TekShader* shader = tek_renderer_get_shape_shader();
	tek_shader_bind(shader);

	Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
	mvp = mat4_transposed(&mvp);

	tek_shader_uniform_mat4(shader, "u_mvp", &mvp, 1);

	u32 vao;
	u32 vbo;

	int loc_pos = tek_shader_get_attrib_loc(shader, "a_position");
	int loc_color = tek_shader_get_attrib_loc(shader, "a_color");

	int size = buffer.size() * sizeof(VertexData);

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, &buffer[0], GL_STATIC_DRAW));

	int stride = sizeof(VertexData);

	GLCall(glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(VertexData, position))));
	GLCall(glEnableVertexAttribArray(loc_pos));

	GLCall(glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, stride,
		(const GLvoid*)(offsetof(VertexData, color))));
	GLCall(glEnableVertexAttribArray(loc_color));	


	GLCall(glDrawArrays(GL_LINES, 0, buffer.size()));

	GLCall(glDisableVertexAttribArray(loc_pos));
	GLCall(glDisableVertexAttribArray(loc_color));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glDeleteBuffers(1, &vbo));
	GLCall(glDeleteVertexArrays(1, &vao));
}

void TekShapebuffer::reset()
{
	buffer.clear();
}