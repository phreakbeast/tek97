#vs

#version 130

attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_mv;
uniform mat4 u_world;

varying vec2 v_uv;
varying float v_affine;
varying vec3 v_normal;
varying vec4 v_color;
varying vec4 v_position;
varying vec3 v_world_position;

#in utils.glsl

uniform DirectionalLight u_direct_light;
uniform vec3 u_eye_world_pos;
uniform int u_use_d_light;
uniform int u_use_p_light;

uniform PointLight u_point_lights[8];
uniform int u_num_pointlights;



void main()
{
	vec4 vertex = u_mvp * vec4(a_position, 1.0);
	v_normal = (u_world * vec4(a_normal, 0.0)).xyz;
	v_position = vec4(a_position,1.0);
	//v_uv = a_uv;
	v_world_position = (u_world * vec4(a_position,1.0)).xyz;
	
	vertex = vertex_snap(vertex, vec2(320.0,240.0));
	gl_Position = vertex;

	vec4 vertex_mv = u_mv * vec4(a_position,1.0);

	vec3 dir_color = vec3(0,0,0);
	vec3 plight_color = vec3(0,0,0);
	vec3 light_color = vec3(1,1,1);

	if(u_use_d_light == 1)
	{
		dir_color = calc_dir_light(u_direct_light,v_normal);
	}
	
	if(u_use_p_light == 1)
	{
		for(int i=0; i<u_num_pointlights; ++i)
		{
			plight_color += calc_point_light(u_point_lights[i],v_world_position);
		}
	}

	if(u_use_d_light == 1 && u_use_p_light != 1)
	{
		light_color = dir_color;
	}
	else if(u_use_d_light != 1 && u_use_p_light == 1)
	{
		light_color = plight_color;
	}
	else if(u_use_d_light == 1 && u_use_p_light == 1)
	{
		light_color = dir_color + plight_color;
	}


	v_color = vec4(light_color,1.0);

	// vertex_mv is the vertex in view-space (i.e. viewMatrix * modelMatrix * vertex)
	float dist = length(vertex_mv);
	float affine = dist + ((vertex.w * 8.0) / dist) * 0.5; // We're going to use this to trick OpenGL into doing perspective-incorrect texture mapping >:)
	v_uv = a_uv * affine; // Passing out modified texture coordinates
	v_affine = affine; // Needed in the fragment shader!
}


#fs

#version 130

varying vec2 v_uv;
varying float v_affine;
varying vec3 v_normal;
varying vec4 v_color;
varying vec4 v_position;
varying vec3 v_world_position;

#in material.glsl

uniform Material u_material;

void main()
{
	vec2 uv_affine = v_uv / v_affine;

	//material color
	vec4 diffuse_color = material_get_diffuse_color(u_material, uv_affine);
	vec4 specular_color = material_get_specular_color(u_material, uv_affine);

	gl_FragColor = vec4(diffuse_color.xyz * v_color.xyz,1.0);
}