#vs

#version 130

attribute vec4 a_position;
attribute vec2 a_uv;
attribute float a_tid;
attribute vec4 a_color;

uniform mat4 u_mvp;

varying vec4 v_position;
varying vec2 v_uv;
varying float v_tid;
varying vec4 v_color;

void main()
{
	v_position = a_position;
	gl_Position = u_mvp * a_position;
	v_uv = a_uv;
	v_tid = a_tid;
	v_color = a_color;
}


#fs

#version 130

varying vec4 v_position;
varying vec2 v_uv;
varying float v_tid;
varying vec4 v_color;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_4;
uniform sampler2D u_texture_5;
uniform sampler2D u_texture_6;
uniform sampler2D u_texture_7;

void main()
{
	if(v_tid > 0.0)
	{
		int tid = int(v_tid - 0.5);
		vec4 tex_color;
		if(tid == 0)
		{
			tex_color = texture(u_texture_0, v_uv.xy);
		}
		else if(tid == 1)
		{
			tex_color = texture(u_texture_1, v_uv.xy);
		}
		else if(tid == 2)
		{
			tex_color = texture(u_texture_2, v_uv.xy);
		}
		else if(tid == 3)
		{
			tex_color = texture(u_texture_3, v_uv.xy);
		}
		else if(tid == 4)
		{
			tex_color = texture(u_texture_4, v_uv.xy);
		}
		else if(tid == 5)
		{
			tex_color = texture(u_texture_5, v_uv.xy);
		}
		else if(tid == 6)
		{
			tex_color = texture(u_texture_6, v_uv.xy);
		}
		else if(tid == 7)
		{
			tex_color = texture(u_texture_7, v_uv.xy);
		}
		gl_FragColor = tex_color;
	}
	else
	{
		gl_FragColor =  v_color;
	}
}