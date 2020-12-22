#include "tek_camera.hpp"

TekCamera::TekCamera(TekCamera::Type type, Mat4 projection)
{
	this->projection = projection;
	this->type = type;
	switch (type)
	{
		case Type::First:
		{
			position = vec3_create(0, 0, 0);
			rotation = vec3_create(0, 0, 0);
		}
			break;
		case Type::Third:
		{
			position = vec3_create(0, 0, 0);
			rotation = vec3_create(40, 0, 0);
			target_position = vec3_create(0, 0, 0);
			target_rotation = vec3_create(0, 0, 0);
			distance = 10;
			min_distance = 2;
			max_distance = 15;
			min_tilt = 15;
			max_tilt = 75;
		}
			break;
		default:
			break;
	}
	calc();
}

void TekCamera::calc()
{
	switch (type)
	{
		case Type::First:
		{
			Mat4 trans = mat4_translate(-position.x, -position.y, -position.z);

			Mat4 rot_x = mat4_rotate(-rotation.x, 1, 0, 0);
			Mat4 rot_y = mat4_rotate(-rotation.y, 0, 1, 0);

			Mat4 rot = mat4_mul2(&rot_x, &rot_y);

			view = mat4_mul2(&rot, &trans);
			view_rot = rot;
		}
			break;
		case Type::Third:
		{
			//calculate camera view
			//calc distance to target point
			float dist_hor = distance * cosf(math_to_radian(rotation.x));
			float dist_vert = distance * sinf(math_to_radian(rotation.x));

			//calc position
			position.y = target_position.y + dist_vert;

			float theta = target_rotation.y;
			float x_offset = dist_hor * sinf(math_to_radian(theta));
			float z_offset = dist_hor * cosf(math_to_radian(theta));

			position.x = target_position.x + x_offset;
			position.z = target_position.z + z_offset;

			//g_cam.rotation.y = 180 - theta;
			rotation.y = theta;

			Mat4 trans = mat4_translate(-position.x, -position.y, -position.z);
			Mat4 rot_x = mat4_rotate(rotation.x, 1, 0, 0);
			Mat4 rot_y = mat4_rotate(-rotation.y, 0, 1, 0);
			Mat4 rot = mat4_mul2(&rot_x, &rot_y);

			view = mat4_mul2(&rot, &trans);
			view_rot = rot;
		}
			break;
		default:
			break;
	}
}

void TekCamera::move(float dir, float speed)
{
	switch (type)
	{
		case Type::First:
		{
			float rad = math_to_radian(rotation.y + dir);
			position.x -= sinf(rad) * speed;
			position.z -= cosf(rad) * speed;
		}
			break;
		case Type::Third:
		{
			float rad = math_to_radian(target_rotation.y + dir);
			target_position.x -= sinf(rad) * speed;
			target_position.z -= cosf(rad) * speed;
		}
			break;
		default:
			break;
	}
}

void TekCamera::move_up(float dir, float speed)
{
	switch (type)
	{
		case Type::First:
		{
			float rad = math_to_radian(rotation.x + dir);
			position.y += sinf(rad) * speed;
		}
			break;
		case Type::Third:
		{
			float rad = math_to_radian(target_rotation.y + dir);
			target_position.y += sinf(rad) * speed;
		}
			break;
		default:
			break;
	}
}

void TekCamera::rotate_x(float val)
{
	switch (type)
	{
		case Type::First:
		{
			rotation.x += val;
			if (rotation.x < -45)
			{
				rotation.x = -45;
			}
			if (rotation.x > 80)
			{
				rotation.x = 80;
			}
		}
			break;
		case Type::Third:
		{
			rotation.x += val;
			if (rotation.x < min_tilt)
			{
				rotation.x = min_tilt;
			}
			if (rotation.x > max_tilt)
			{
				rotation.x = max_tilt;
			}
		}
			break;
		default:
			break;
	}
}

void TekCamera::rotate_y(float val)
{
	switch (type)
	{
		case Type::First:
		{
			rotation.y += val;
			if (rotation.y > 360.0f)
			{
				rotation.y -= 360.0f;
			}
			if (rotation.y < 0.0f)
			{
				rotation.y += 360.0f;
			}
		}
			break;
		case Type::Third:
		{
			target_rotation.y += val;
			if (target_rotation.y > 360.0f)
			{
				target_rotation.y -= 360.0f;
			}
			if (target_rotation.y < 0.0f)
			{
				target_rotation.y += 360.0f;
			}
		}
			break;
		default:
			break;
	}
}

void TekCamera::zoom(float val)
{
	switch (type)
	{
		case Type::Third:
		{
			distance += val;
			if (distance > max_distance)
			{
				distance = max_distance;
			}

			if (distance < min_distance)
			{
				distance = min_distance;
			}
		}
			break;
		default:
			break;
	}
}

