#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <numbers>

template <typename T> constexpr auto deg2rad(T degrees) noexcept { return degrees * (static_cast<T>(M_PI) / static_cast<T>(180)); }
template <typename T> constexpr auto rad2deg(T radians) noexcept { return radians * (static_cast<T>(180) / static_cast<T>(M_PI)); }

namespace PwnBoxFramework
{
	struct Vector2
	{
		float x, y;
		Vector2() noexcept : x(0.0f), y(0.0f) {}
		Vector2(float x, float y) noexcept : x(x), y(y) {}

		const Vector2& operator=(const Vector2& v)
		{
			this->x = v.x;
			this->y = v.y; 
			return *this;
		}

		const Vector2& operator()(const Vector2& v) { return operator=(Vector2(v)); }
	};

	struct Vector3
	{
		float x, y, z;
		Vector3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}
		constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
		Vector3& operator=(const Vector3& v) noexcept
		{
			this->x = x;
			this->y = y;
			this->z = z;
			return *this;
		}

		constexpr auto notNull() const noexcept
		{
			return x || y || z;
		}

		friend constexpr auto operator==(const Vector3& a, const Vector3& b) noexcept
		{
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}

		friend constexpr auto operator!=(const Vector3& a, const Vector3& b) noexcept
		{
			return !(a == b);
		}

		constexpr Vector3& operator=(const float array[3]) noexcept
		{
			x = array[0];
			y = array[1];
			z = array[2];
			return *this;
		}

		constexpr Vector3& operator+=(const Vector3& v) noexcept
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		constexpr Vector3& operator+=(float f) noexcept
		{
			x += f;
			y += f;
			z += f;
			return *this;
		}

		constexpr Vector3& operator-=(const Vector3& v) noexcept
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		constexpr Vector3& operator-=(float f) noexcept
		{
			x -= f;
			y -= f;
			z -= f;
			return *this;
		}

		friend constexpr auto operator-(const Vector3& a, const Vector3& b) noexcept
		{
			return Vector3{ a.x - b.x, a.y - b.y, a.z - b.z };
		}

		auto length() const noexcept { return std::sqrt(x * x + y * y + z * z); }

		auto distTo(const Vector3& v) const noexcept
		{
			return (*this - v).length();
		}

		auto toAngle() const noexcept
		{
			return Vector3{ ::rad2deg(std::atan2(-z, std::hypot(x, y))),
							::rad2deg(std::atan2(y, x)),
						   0.0f };
		}

		Vector3& operator()(const Vector3& v) noexcept { return *this = Vector3(v.x,v.y,v.z); }

		const auto& DotProduct(const Vector3& v) noexcept { return ((this->x * v.x) + (this->y * v.y) + (this->z * v.z)); }
		
		const auto& CrossProduct(const Vector3& v) noexcept
		{
			*this = Vector3(
				((this->y*v.z) - (this->z*v.y)) ,
				-1.0f * (((this->x * v.z) - (this->z * v.x))),
				((this->x * v.y) - (this->y * v.x))
			);
			return *this;
		}

		Vector3& normalize() noexcept
		{
			x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
			y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
			z = 0.0f;
			return *this;
		}

		auto normalizeInPlace() noexcept
		{
			float radius = std::sqrt(x * x + y * y + z * z);
			float iradius = 1.f / (radius + 1.192092896e-07F /* FLT_EPSILON */);

			x *= iradius;
			y *= iradius;
			z *= iradius;

			return radius;
		}

		static auto fromAngle(const Vector3& angle) noexcept
		{
			return Vector3{ std::cos(::deg2rad(angle.x)) * std::cos(::deg2rad(angle.y)),
						   std::cos(::deg2rad(angle.x)) * std::sin(::deg2rad(angle.y)),
						  -std::sin(::deg2rad(angle.x)) };
		}

		const auto& GetInverseSqrt(float value)
		{
			long i;
			float x2 = value * 0.5f,y = value;
			const float threehalfs = 1.5f;
			i = *(long*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;
			y = y * (threehalfs - (x2 * y * y));
			return y;
		}
	};
}