#include "tek_quat.hpp"

#include "tek_math_functions.hpp"

static inline float norm_helper(Quat q)
{
	float result;
	result = (q.x * q.x);
	result = (result + (q.y * q.y));
	result = (result + (q.z * q.z));
	result = (result + (q.w * q.w));
	return result;
}

Quat::Quat()
{
	x= 0;
	y= 0;
	z= 0;
	w= 0;
}

Quat::Quat(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

const float Quat::length() const
{
	return sqrtf(norm_helper(*this));
}

const Quat Quat::normalized() const
{
	float len_sqr = norm_helper(*this);
	float len_inv = math_rsqrtf(len_sqr);
	return (*this) * len_inv;
}

const Vec3 Quat::to_euler() const
{
	Vec3 res = {
			atan2f(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z),
			atan2f(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z),
			asinf(2 * x * y + 2 * z * w)
	};
	return res;
}


Vec3 Quat::rotate(Vec3 v, Quat q)
{
	float tmpX, tmpY, tmpZ, tmpW;
	tmpX = (((q.w * v.x) + (q.y * v.z)) - (q.z * v.y));
	tmpY = (((q.w * v.y) + (q.z * v.x)) - (q.x * v.z));
	tmpZ = (((q.w * v.z) + (q.x * v.y)) - (q.y * v.x));
	tmpW = (((q.x * v.x) + (q.y * v.y)) + (q.z * v.z));

	Vec3 res;
	res.x = ((((tmpW * q.x) + (tmpX * q.w)) - (tmpY * q.z)) + (tmpZ * q.y));
	res.y = ((((tmpW * q.y) + (tmpY * q.w)) - (tmpZ * q.x)) + (tmpX * q.z));
	res.z = ((((tmpW * q.z) + (tmpZ * q.w)) - (tmpX * q.y)) + (tmpY * q.x));

	return res;
}

Quat Quat::rotation(Vec3 v1, Vec3 v2)
{
	float cosHalfAngleX2, recipCosHalfAngleX2;
	float dot = vec3_dot(v1, v2);
	cosHalfAngleX2 = sqrtf((2.0f * (1.0f + dot)));
	recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
	Vec3 tmp = vec3_cross(v1, v2);
	tmp = vec3_mul(tmp, recipCosHalfAngleX2);
	Quat res;
	res.x = tmp.x;
	res.y = tmp.y;
	res.z = tmp.z;
	res.w = cosHalfAngleX2 * 0.5f;
	return res;
}

Quat Quat::rotation_axis(float num, Vec3 v)
{
	float angle = num * 0.5f;
	Vec3 t = vec3_mul(v, (sinf(angle)));
	Quat res;
	res.x = t.x;
	res.y = t.y;
	res.z = t.z;
	res.w = cosf(angle);
	return res;
}

Quat Quat::from_euler(Vec3 euler)
{
	Quat pitch;
	pitch.x = 1;
	pitch.y = 0;
	pitch.z = 0;
	pitch.w = euler.x;

	Quat yaw;
	yaw.x = 0;
	yaw.y = 1;
	yaw.z = 0;
	yaw.w = euler.y;

	Quat roll;
	roll.x = 0;
	roll.y = 0;
	roll.z = 1;
	roll.w = euler.z;

	Quat t = pitch * yaw * roll;
	return t;
}

const Quat Quat::operator+(const Quat& q2) const
{
	Quat res;
	res.x = x + q2.x;
	res.y = y + q2.y;
	res.z = z + q2.z;
	res.w = w + q2.w;
	return res;
}

const Quat Quat::operator-(const Quat& q2) const
{
	Quat res;
	res.x = x - q2.x;
	res.y = y - q2.y;
	res.z = z - q2.z;
	res.w = w - q2.w;
	return res;
}

const Quat Quat::operator*(float num) const
{
	Quat res;
	res.x = x * num;
	res.y = y * num;
	res.z = z * num;
	res.w = w * num;
	return res;
}

const Quat Quat::operator*(const Quat& q2) const
{
	Quat res;

	res.x = (((w * q2.x) + (x * q2.w)) + (y * q2.z)) - (z * q2.y);
	res.y = (((w * q2.y) + (y * q2.w)) + (z * q2.x)) - (x * q2.z);
	res.z = (((w * q2.z) + (z * q2.w)) + (x * q2.y)) - (y * q2.x);
	res.w = (((w * q2.w) - (x * q2.x)) - (y * q2.y)) - (z * q2.z);

	return res.normalized();
}
