#vs

#version 130

attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_world;

varying vec2 v_uv;	
varying vec3 v_normal;
varying vec4 v_position;
varying vec3 v_world_position;

void main()
{
	gl_Position = u_mvp * vec4(a_position, 1.0);
	
	v_position = vec4(a_position,1.0);
	v_uv = a_uv;
	v_normal = (u_world * vec4(a_normal, 0.0)).xyz;
	v_world_position = (u_world * vec4(a_position,1.0)).xyz;
}


#fs

#version 130

varying vec2 v_uv;
varying vec3 v_normal;
varying vec4 v_position;
varying vec3 v_world_position;

uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture,v_uv.xy);
	gl_FragColor = color;
}