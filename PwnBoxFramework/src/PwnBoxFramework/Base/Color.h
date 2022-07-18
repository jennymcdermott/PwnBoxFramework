#pragma once

struct Color
{
	float red, green, blue, alpha;
	Color() noexcept : red(0.0f),green(0.0f),blue(0.0f),alpha(1.0f) {}
	constexpr Color(float red, float green, float blue,float alpha)
		noexcept : red(red), green(green), blue(blue),alpha(alpha)  {  }
};