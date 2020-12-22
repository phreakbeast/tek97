#ifndef _TEK_QUAT_HPP
#define _TEK_QUAT_HPP

#include "tek_vec3.hpp"

struct Quat
{
	float x;
	float y;
	float z;
	float w;

	Quat();
	Quat(float x, float y, float z, float w);

	const float length() const;

	const Quat normalized() const;

	const Vec3 to_euler() const;

	static Vec3 rotate(Vec3 v, Quat q);
	static Quat rotation(Vec3 v1, Vec3 v2);
	static Quat rotation_axis(float num, Vec3 v);
	static Quat from_euler(Vec3 euler);

	const Quat operator+(const Quat& q2) const;

	const Quat operator-(const Quat& q2) const;

	const Quat operator*(const Quat& q2) const;

	const Quat operator*(float num) const;
};


#endif