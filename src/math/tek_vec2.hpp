#ifndef _TEK_VEC2_HPP
#define _TEK_VEC2_HPP

namespace tek
{

	struct Vec2
	{
		float x;
		float y;

		Vec2();

		Vec2(float a);

		Vec2(float x, float y);

		const float length() const;

		const Vec2 normalized() const;

		const float dot(Vec2 v2) const;

		const float distance(Vec2 v2) const;
	};

	const Vec2 operator+(const Vec2& v1, const Vec2& v2);
	const Vec2 operator-(const Vec2& v1, const Vec2& v2);
	const Vec2 operator*(const Vec2& v, float num);

	Vec2 vec2_create(float x, float y);

	const float vec2_length(Vec2 v);

	const Vec2 vec2_normalized(Vec2 v);

	const float vec2_dot(Vec2 v1, Vec2 v2);

	const float vec2_distance(Vec2 v1, Vec2 v2);

	const Vec2 vec2_add(const Vec2 v1, const Vec2 v2);

	const Vec2 vec2_sub(const Vec2 v1, const Vec2 v2);

	const Vec2 vec2_mul(const Vec2 v, float num);

	const Vec2 math_get_point_on_circle(float rad, float angle);

}
#endif
