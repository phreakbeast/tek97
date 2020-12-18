#ifndef _TEK_CAMERA_HPP
#define _TEK_CAMERA_HPP

#include "../math/tek_math.hpp"

typedef enum
{
	TEK_CAM_FIRST,
	TEK_CAM_THIRD,
} TekCameraType;

typedef struct
{
	TekCameraType type;

	Vec3 position;
	Vec3 rotation;

	Mat4 view;
	Mat4 view_rot;
	Mat4 projection;

	Vec3 target_position;
	Vec3 target_rotation;

	float distance;
	float min_distance;
	float max_distance;
	float min_tilt;
	float max_tilt;
} TekCamera;

TekCamera tek_cam_create(TekCameraType type, Mat4 projection);

void tek_cam_calc(TekCamera *cam);

void tek_cam_move(TekCamera *cam, float dir, float speed);

void tek_cam_move_up(TekCamera *cam, float dir, float speed);

void tek_cam_rotate_x(TekCamera *cam, float val);

void tek_cam_rotate_y(TekCamera *cam, float val);

void tek_cam_zoom(TekCamera *cam, float val);

#endif
