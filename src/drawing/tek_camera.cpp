#include "tek_camera.hpp"

TekCamera tek_cam_create(CameraType type, Mat4 projection)
{
	TekCamera res;
	res.projection = projection;
	res.type = type;
	switch (type)
	{
		case TEK_CAMERA_FIRST:
		{
			res.position = vec3_create(0, 0, 0);
			res.rotation = vec3_create(0, 0, 0);
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			res.position = vec3_create(0, 0, 0);
			res.rotation = vec3_create(40, 0, 0);
			res.target_position = vec3_create(0, 0, 0);
			res.target_rotation = vec3_create(0, 0, 0);
			res.distance = 10;
			res.min_distance = 2;
			res.max_distance = 15;
			res.min_tilt = 15;
			res.max_tilt = 75;
		}
			break;
		default:
			break;
	}
	tek_cam_calc(&res);
	return res;
}

void tek_cam_calc(TekCamera* cam)
{
	switch (cam->type)
	{
		case TEK_CAMERA_FIRST:
		{
			Mat4 trans = mat4_translate(-cam->position.x, -cam->position.y, -cam->position.z);

			Mat4 rot_x = mat4_rotate(-cam->rotation.x, 1, 0, 0);
			Mat4 rot_y = mat4_rotate(-cam->rotation.y, 0, 1, 0);

			Mat4 rot = mat4_mul2(&rot_x, &rot_y);

			cam->view = mat4_mul2(&rot, &trans);
			cam->view_rot = rot;
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			//calculate camera view
			//calc distance to target point
			float dist_hor = cam->distance * cosf(math_to_radian(cam->rotation.x));
			float dist_vert = cam->distance * sinf(math_to_radian(cam->rotation.x));

			//calc position
			cam->position.y = cam->target_position.y + dist_vert;

			float theta = cam->target_rotation.y;
			float x_offset = dist_hor * sinf(math_to_radian(theta));
			float z_offset = dist_hor * cosf(math_to_radian(theta));

			cam->position.x = cam->target_position.x + x_offset;
			cam->position.z = cam->target_position.z + z_offset;

			//g_cam.rotation.y = 180 - theta;
			cam->rotation.y = theta;

			Mat4 trans = mat4_translate(-cam->position.x, -cam->position.y, -cam->position.z);
			Mat4 rot_x = mat4_rotate(cam->rotation.x, 1, 0, 0);
			Mat4 rot_y = mat4_rotate(-cam->rotation.y, 0, 1, 0);
			Mat4 rot = mat4_mul2(&rot_x, &rot_y);

			cam->view = mat4_mul2(&rot, &trans);
			cam->view_rot = rot;
		}
			break;
		default:
			break;
	}
}

void tek_cam_move(TekCamera* cam, float dir, float speed)
{
	switch (cam->type)
	{
		case TEK_CAMERA_FIRST:
		{
			float rad = math_to_radian(cam->rotation.y + dir);
			cam->position.x -= sinf(rad) * speed;
			cam->position.z -= cosf(rad) * speed;
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			float rad = math_to_radian(cam->target_rotation.y + dir);
			cam->target_position.x -= sinf(rad) * speed;
			cam->target_position.z -= cosf(rad) * speed;
		}
			break;
		default:
			break;
	}
}

void tek_cam_move_up(TekCamera* cam, float dir, float speed)
{
	switch (cam->type)
	{
		case TEK_CAMERA_FIRST:
		{
			float rad = math_to_radian(cam->rotation.x + dir);
			cam->position.y += sinf(rad) * speed;
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			float rad = math_to_radian(cam->target_rotation.y + dir);
			cam->target_position.y += sinf(rad) * speed;
		}
			break;
		default:
			break;
	}
}

void tek_cam_rotate_x(TekCamera* cam, float val)
{
	switch (cam->type)
	{
		case TEK_CAMERA_FIRST:
		{
			cam->rotation.x += val;
			if (cam->rotation.x < -45)
			{
				cam->rotation.x = -45;
			}
			if (cam->rotation.x > 80)
			{
				cam->rotation.x = 80;
			}
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			cam->rotation.x += val;
			if (cam->rotation.x < cam->min_tilt)
			{
				cam->rotation.x = cam->min_tilt;
			}
			if (cam->rotation.x > cam->max_tilt)
			{
				cam->rotation.x = cam->max_tilt;
			}
		}
			break;
		default:
			break;
	}
}

void tek_cam_rotate_y(TekCamera* cam, float val)
{
	switch (cam->type)
	{
		case TEK_CAMERA_FIRST:
		{
			cam->rotation.y += val;
			if (cam->rotation.y > 360.0f)
			{
				cam->rotation.y -= 360.0f;
			}
			if (cam->rotation.y < 0.0f)
			{
				cam->rotation.y += 360.0f;
			}
		}
			break;
		case TEK_CAMERA_THIRD:
		{
			cam->target_rotation.y += val;
			if (cam->target_rotation.y > 360.0f)
			{
				cam->target_rotation.y -= 360.0f;
			}
			if (cam->target_rotation.y < 0.0f)
			{
				cam->target_rotation.y += 360.0f;
			}
		}
			break;
		default:
			break;
	}
}

void tek_cam_zoom(TekCamera* cam, float val)
{
	switch (cam->type)
	{
		case TEK_CAMERA_THIRD:
		{
			cam->distance += val;
			if (cam->distance > cam->max_distance)
			{
				cam->distance = cam->max_distance;
			}

			if (cam->distance < cam->min_distance)
			{
				cam->distance = cam->min_distance;
			}
		}
			break;
		default:
			break;
	}
}

