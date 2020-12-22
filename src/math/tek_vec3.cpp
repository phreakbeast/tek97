#include "tek_vec3.hpp"

#include "tek_math_functions.hpp"

Vec3::Vec3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float a)
{
	this->x = a;
	this->y = a;
	this->z = a;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const float Vec3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

const Vec3 Vec3::normalized() const
{
	float len = length();
	if (len == 0)
	{
		len = 0.0000001f;
	}

	Vec3 res = {
			x / len,
			y / len,
			z / len
	};
	return res;
}

const Vec3 Vec3::negate() const
{
	Vec3 res = {-x, -y, -z};
	return res;
}

const float Vec3::dot(Vec3 v2) const
{
	return x * v2.x + y * v2.y + z * v2.z;
}

const Vec3 Vec3::cross(Vec3 v2) const
{
	Vec3 res = {
			y * v2.z - z * v2.y,
			z * v2.x - x * v2.z,
			x * v2.y - y * v2.x
	};

	return res;
}

const float Vec3::distance(Vec3 v2) const
{
	float a = x - v2.x;
	float b = y - v2.y;
	float c = z - v2.z;
	return sqrtf(a * a + b * b + c * c);
}

const Vec3 Vec3::lerp(Vec3 v2, float t) const
{
	Vec3 res = {
			x + t * (v2.x - x),
			y + t * (v2.y - y),
			z + t * (v2.z - z)
	};
	return res;
}

const Vec3 Vec3::operator+(const Vec3& v2)
{
	Vec3 res = {
			x + v2.x,
			y + v2.y,
			z + v2.z
	};
	return res;
}

const Vec3 Vec3::operator-(const Vec3& v2)
{
	Vec3 res = {
			x - v2.x,
			y - v2.y,
			z - v2.z
	};
	return res;
}

const Vec3 Vec3::operator*(float num)
{
	Vec3 res = {
			x * num,
			y * num,
			z * num
	};
	return res;
}

Vec3 math_calc_normals(Vec3 p0, Vec3 p1, Vec3 p2)
{
	Vec3 v1 = vec3_sub(p1, p0);
	Vec3 v2 = vec3_sub(p2, p0);
	Vec3 normal = vec3_cross(v1,v2);
	normal = vec3_normalized(normal);

	return normal;
}

Vec3 vec3_create(float x, float y, float z)
{
	Vec3 res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

const float vec3_length(Vec3 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

const Vec3 vec3_normalized(Vec3 v)
{
	float len = vec3_length(v);
	if (len == 0)
	{
		len = 0.0000001f;
	}

	Vec3 res = {
			v.x / len,
			v.y / len,
			v.z / len
	};
	return res;
}

const Vec3 vec3_negate(Vec3 v)
{
	Vec3 res = {-v.x, -v.y, -v.z};
	return res;
}

const float vec3_dot(Vec3 v1, Vec3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const Vec3 vec3_cross(Vec3 v1, Vec3 v2)
{
	Vec3 res = {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
	};

	return res;
}

const float vec3_distance(Vec3 v1, Vec3 v2)
{
	float a = v1.x - v2.x;
	float b = v1.y - v2.y;
	float c = v1.z - v2.z;
	return sqrtf(a * a + b * b + c * c);
}

const Vec3 vec3_lerp(Vec3 v1, Vec3 v2, float t)
{
	Vec3 res = {
			v1.x + t * (v2.x - v1.x),
			v1.y + t * (v2.y - v1.y),
			v1.z + t * (v2.z - v1.z)
	};
	return res;
}

const Vec3 vec3_add(const Vec3 v1, const Vec3 v2)
{
	Vec3 res = {
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z
	};
	return res;
}

const Vec3 vec3_sub(const Vec3 v1, const Vec3 v2)
{
	Vec3 res = {
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z
	};
	return res;
}

const Vec3 vec3_mul(const Vec3 v1, float num)
{
	Vec3 res = {
			v1.x * num,
			v1.y * num,
			v1.z * num
	};
	return res;
}



