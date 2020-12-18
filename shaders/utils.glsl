
struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

struct PointLight
{
	vec3 position;
	vec3 color;
	float range;
};

vec3 calc_dir_light(DirectionalLight dlight, vec3 vert_normal)
{
	vec3 dir_color = vec3(0,0,0);
	vec3 light_dir = vec3(-dlight.direction);
	vec3 light_ambient = vec3(dlight.intensity, dlight.intensity, dlight.intensity);
	vec3 light_diffuse = dlight.color;

	vec3 n = normalize(vert_normal);
	float cosTheta = clamp( dot( n, light_dir ), 0,1 );
	
	dir_color = (vec4(light_ambient, 1.0) + vec4(light_diffuse, 1.0) * vec4(vec3(1,1,1) * cosTheta, 1.0)).xyz;
	return dir_color;
}

vec3 calc_point_light(PointLight plight, vec3 vert_position)
{
	vec3 res_color = vec3(0,0,0);
	vec3 point_pos = plight.position;
	vec3 point_color = plight.color;
	float point_range = plight.range;

	float dist = distance(vert_position, point_pos);
	if(dist > point_range)
	{
		return res_color;
	}

	float percentage = 1.0 - dist / point_range;
	res_color = point_color * percentage;
	return res_color;
}

vec4 calculate_light(DirectionalLight light, vec3 normal, vec3 direction, vec3 eye_world_pos, vec3 world_pos, float specular_intensity, float specular_power, float shadow_factor)
{
	vec4 ambient_color = vec4(light.color * light.intensity, 1.0);
	float diffuse_factor = dot(normalize(normal), -direction);

	vec4 diffuse_color = vec4(0,0,0,1);
	vec4 specular_color = vec4(0,0,0,1);

	if(diffuse_factor > 0)
	{
		diffuse_color = vec4(light.color, 1.0) * light.intensity * diffuse_factor;

		vec3 vertex_to_eye = normalize(eye_world_pos - world_pos);
		vec3 light_reflect = normalize(reflect(direction,normal));
		float specular_factor = dot(vertex_to_eye, light_reflect);
		if(specular_factor > 0)
		{
			specular_factor = pow(specular_factor, specular_power);
			specular_color = vec4(light.color * specular_intensity * specular_factor, 1.0);
		}
	}	
	
	return ambient_color * (diffuse_color + specular_color);
}

vec4 vertex_snap(vec4 vertex, vec2 resolution)
{
	vec4 snappedPos = vertex;
	snappedPos.xyz = vertex.xyz / vertex.w; // convert to normalised device coordinates (NDC)
	snappedPos.xy = floor(resolution * snappedPos.xy) / resolution; // snap the vertex to the lower-resolution grid
	snappedPos.xyz *= vertex.w; // convert back to projection-space
	return snappedPos;
}