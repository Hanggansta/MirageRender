#ifndef VEC3_H
#define VEC3_H

// std includes
#include <string>

// mirage includes
#include "macros.h"
#include "math.h"

namespace mirage
{

	struct vec3;
	extern vec3 operator+(const float f, const vec3 &other);
	extern vec3 operator-(const float f, const vec3 &other);
	extern vec3 operator*(const float f, const vec3 &other);
	extern vec3 operator/(const float f, const vec3 &other);

	struct vec3
	{
		float x, y, z;

		vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) { }

		std::string toString() const;

		inline bool operator==(const vec3 &other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		inline bool operator!=(const vec3 &other) const
		{
			return !(*this == other);
		}

		inline vec3 &operator+=(const vec3 &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		inline vec3 &operator+=(const float f)
		{
			x += f;
			y += f;
			z += f;
			return *this;
		}

		inline vec3 &operator-=(const vec3 &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		inline vec3 &operator-=(const float f)
		{
			x -= f;
			y -= f;
			z -= f;
			return *this;
		}

		inline vec3 &operator*=(const vec3 &other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		inline vec3 &operator*=(const float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		inline vec3 &operator/=(const vec3 &other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		inline vec3 &operator/=(const float f)
		{
			float f_inv = 1.0f / f;
			x *= f_inv;
			y *= f_inv;
			z *= f_inv;
			return *this;
		}

		inline vec3 operator+(const vec3 &other) const
		{
			return vec3(x + other.x, y + other.y, z + other.z);
		}

		inline vec3 operator+(const float f) const
		{
			return vec3(x + f, y + f, z + f);
		}

		inline vec3 operator-(const vec3 &other) const
		{
			return vec3(x - other.x, y - other.y, z - other.z);
		}

		inline vec3 operator-(const float f) const
		{
			return vec3(x - f, y - f, z - f);
		}

		inline vec3 operator*(const vec3 &other) const
		{
			return vec3(x * other.x, y * other.y, z * other.z);
		}

		inline vec3 operator*(const float f) const
		{
			return vec3(x * f, y * f, z * f);
		}

		inline vec3 operator/(const vec3 &other) const
		{
			return vec3(x / other.x, y / other.y, z / other.z);
		}

		inline vec3 operator/(const float f) const
		{
			float f_inv = 1.0f / f;
			return vec3(x * f_inv, y * f_inv, z * f_inv);
		}

		inline float operator[](int i) const
		{
			return (&x)[i];
		}

		inline float &operator[](int i)
		{
			return (&x)[i];
		}

		inline vec3 negate() const
		{
			return vec3(-x, -y, -z);
		}

		inline float length() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		inline vec3 normalize() const
		{
			float l = 1.0f / length();
			return vec3(x * l, y * l, z * l);
		}

		static inline vec3 orthogonal(const vec3 &v)
		{
			float x = std::abs(v.x);
			float y = std::abs(v.y);
			float z = std::abs(v.z);

			vec3 other = x < y ? (x < z ? vec3(1, 0, 0) : vec3(0, 0, 1)) : (y < z ? vec3(0, 1, 0) : vec3(0, 0, 1));
			return vec3::cross(v, other);
		}

		static inline float dot(const vec3 &left, const vec3 &right)
		{
			return left.x * right.x + left.y * right.y + left.z * right.z;
		}

		static inline vec3 cross(const vec3 &left, const vec3 &right)
		{
			return vec3(left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y);
		}

		static inline vec3 reflect(const vec3 &I, const vec3 &N)
		{
			return I - (N * dot(N, I) * 2.0f);
		}

		static inline vec3 refract(const vec3 &I, const vec3 &N, float eta)
		{
			float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
			if (k < 0.0f)
				return vec3();
			else
				return eta * I - (eta * dot(N, I) + std::sqrt(k)) * N;
		}

		static inline vec3 sampleHemisphere(const vec3 &N)
		{
			float r1 = pseudorand02pi(); // Spherical coordinates
			float r2 = pseudorand();
			float r2s = std::sqrt(r2);
			vec3 w = N; // w = normal
			vec3 u = (cross((std::abs(w.x) > 0.1f ? vec3(0, 1) : vec3(1)), w)).normalize(); // u is perpendicular to w
			vec3 v = cross(w, u); // v is perpendicular to u and w
			return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalize();
		}

		static inline vec3 sampleHemisphere(const vec3 &N, float scalar, float chance)
		{
			std::uniform_real_distribution<float>brdf_dis(0.0f, scalar);

			float r1 = pseudorand02pi(); // Spherical coordinates
			float r2;

			if (pseudorand() < chance) // Importance sampling, should we bias the distribution or not?
				r2 = brdf_dis(g_randomGen);
			else
				r2 = pseudorand();

			float r2s = std::sqrt(r2);
			vec3 w = N; // w = normal
			vec3 u = (cross((std::abs(w.x) > 0.1f ? vec3(0, 1) : vec3(1)), w)).normalize(); // u is perpendicular to w
			vec3 v = cross(w, u); // v is perpendicular to u and w
			return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalize();
		}

		static inline vec3 powv(const vec3 &v, float f)
		{
			auto x = std::pow(v.x, f);
			auto y = std::pow(v.y, f);
			auto z = std::pow(v.z, f);

			return vec3(x, y, z);
		}

		static inline vec3 clampv(const vec3 &v, float min, float max)
		{
			auto x = clampf(v.x, min, max);
			auto y = clampf(v.y, min, max);
			auto z = clampf(v.z, min, max);

			return vec3(x, y, z);
		}

	};

	inline vec3 operator+(const float f, const vec3 &other)
	{
		return other + f;
	}

	inline vec3 operator-(const float f, const vec3 &other)
	{
		return other - f;
	}

	inline vec3 operator*(const float f, const vec3 &other)
	{
		return other * f;
	}

	inline vec3 operator/(const float f, const vec3 &other)
	{
		return vec3(f / other.x, f / other.y, f / other.z);
	}

}

#endif // VEC3_H
