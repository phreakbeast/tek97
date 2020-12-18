#vs

#version 130

attribute vec4 a_position;
attribute vec2 a_uv;
attribute vec4 a_color;

uniform mat4 u_mvp;

varying vec4 v_position;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{
	v_position = u_mvp * a_position;
	gl_Position = v_position;
	v_uv = a_uv;
	v_color = a_color;
}

#fs

#version 130

varying vec4 v_position;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D u_texture;
uniform int u_use_texture;

void main()
{
	vec4 tex_color = v_color;
	if(u_use_texture == 1)
	{
		tex_color = texture2D(u_texture, v_uv.xy);
	}
	
	gl_FragColor = tex_color;
}