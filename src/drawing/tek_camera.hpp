#ifndef _TEK_CAMERA_HPP
#define _TEK_CAMERA_HPP

#include "../math/tek_math.hpp"

class TekCamera
{
public:
    enum class Type
    {
	First,
	Third,
    };
    
    Type type;
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

    TekCamera(Type type, Mat4 projection);
    void calc();
    void move(float dir, float speed);
    void move_up(float dir, float speed);
    void rotate_x(float val);
    void rotate_y(float val);
    void zoom(float val);
};

#endif
