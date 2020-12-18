#vs

#version 130

attribute vec3 a_position;
attribute vec3 a_color;


uniform mat4 u_mvp;

varying vec4 v_position;
varying vec3 v_color;


void main()
{
	v_position = u_mvp * vec4(a_position,1.0);
	gl_Position = v_position;
	v_color = a_color;
}

#fs

#version 130

varying vec4 v_position;
varying vec3 v_color;

void main()
{		
	gl_FragColor = vec4(v_color, 1.0);
}