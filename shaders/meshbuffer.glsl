#vs

#version 130

attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec3 a_normal;
attribute vec4 a_flags;
attribute mat4 a_model;

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_world;

varying vec2 v_uv;
varying float v_affine;
varying vec3 v_normal;
varying vec4 v_color;
varying vec4 v_position;
varying vec3 v_world_position;
varying vec4 v_flags;

#in utils.glsl

uniform DirectionalLight u_direct_light;
uniform vec3 u_eye_world_pos;
uniform int u_use_d_light;
uniform int u_use_p_light;

uniform PointLight u_point_lights[8];
uniform int u_num_pointlights;



void main()
{	
	v_normal = (a_model * vec4(a_normal, 0.0)).xyz;
	v_position = vec4(a_position,1.0);
	v_flags = a_flags;
	v_world_position = (a_model * vec4(a_position,1.0)).xyz;

	vec4 vertex = u_p * u_v * a_model * vec4(a_position, 1.0);
	int bb_flag = int(a_flags.y);
	if(bb_flag == 1 || bb_flag == 2)
	{
		mat4 model_view = u_v * a_model;

		//first column
		model_view[0][0] = 1.0;
		model_view[0][1] = 0.0;
		model_view[0][2] = 0.0;

		if(bb_flag == 2)
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

		vertex = u_p * model_view * vec4(a_position, 1.0);
	}


	vertex = vertex_snap(vertex, vec2(320.0,240.0));
	
	gl_Position = vertex;
	vec4 vertex_mv = u_v * a_model * vec4(a_position,1.0);

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
varying vec4 v_flags;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_4;
uniform sampler2D u_texture_5;
uniform sampler2D u_texture_6;
uniform sampler2D u_texture_7;

struct Material
{
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;

	int has_diffuse_map;
	int has_specular_map;

	int diffuse_map;
	int specular_map;
};

vec4 material_get_diffuse_color(Material mat, vec2 uv)
{
	vec4 res_color;
	if(mat.has_diffuse_map == 1)
	{
		res_color = texture2D(u_texture_0, uv.xy);
	}
	else
	{
		res_color = mat.diffuse_color + mat.ambient_color;
	}
	return res_color; 
}

vec4 material_get_specular_color(Material mat, vec2 uv)
{
	vec4 res_color;
	
	if(mat.has_specular_map == 1)
	{
		res_color = texture2D(u_texture_0, uv.xy);
	}
	else
	{
		res_color = mat.specular_color;
	}
	
	return res_color; 
}

uniform Material u_material_0;
uniform Material u_material_1;
uniform Material u_material_2;
uniform Material u_material_3;
uniform Material u_material_4;
uniform Material u_material_5;
uniform Material u_material_6;
uniform Material u_material_7;

void main()
{
	vec2 uv_affine = v_uv / v_affine;

	Material mat;
	
	highp int mid = int(v_flags.x);
	if(mid == 0)
	{
		mat = u_material_0;
	}
	else if(mid == 1)
	{
		mat = u_material_1;
	}
	else if(mid == 2)
	{
		mat = u_material_2;
	}
	else if(mid == 3)
	{
		mat = u_material_3;
	}
	else if(mid == 4)
	{
		mat = u_material_4;
	}
	else if(mid == 5)
	{
		mat = u_material_5;
	}
	else if(mid == 6)
	{
		mat = u_material_6;
	}
	else if(mid == 7)
	{
		mat = u_material_7;
	}

	//material color
	vec4 diffuse_color = material_get_diffuse_color(mat, uv_affine);
	vec4 specular_color = material_get_specular_color(mat, uv_affine);

	gl_FragColor = vec4(diffuse_color.xyz * v_color.xyz,1.0);
}