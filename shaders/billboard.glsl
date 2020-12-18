#vs
#version 130

attribute vec3 a_position;
attribute vec2 a_uv;

uniform mat4 u_mv;
uniform mat4 u_p;

uniform int u_spherical;

varying vec4 v_position;
varying vec2 v_uv;
varying vec4 v_color;


void main()
{
	mat4 model_view = u_mv;

	//first column
	model_view[0][0] = 1.0;
	model_view[0][1] = 0.0;
	model_view[0][2] = 0.0;

	if(u_spherical == 1)
	{
		//second column
		model_view[1][0] = 0.0;
		model_view[1][1] = 1.0;
		model_view[1][2] = 0.0;
	}

	//third column
	model_view[2][0] = 0.0;
	model_view[2][1] = 0.0;
	model_view[2][2] = 1.0;

	vec4 p = model_view * vec4(a_position, 1.0);

	v_position = u_p * p;
	gl_Position = v_position;
	v_uv = a_uv;

	v_color = vec4(1,1,1,1);
}

#fs
#version 130

uniform sampler2D u_texture;

varying vec4 v_position;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{
	vec4 tex_color = texture2D(u_texture, v_uv);
	gl_FragColor = tex_color * v_color;
}