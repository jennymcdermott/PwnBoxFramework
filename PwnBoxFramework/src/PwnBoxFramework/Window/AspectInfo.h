#pragma once

namespace PwnBoxFramework
{
	struct AspectRatio
	{
		int width, height;
		AspectRatio() noexcept : width(0), height(0) {}
		AspectRatio(int width, int height) noexcept : width(width), height(height) { }

		AspectRatio operator=(const AspectRatio& a) noexcept
		{
			this->width = a.width;
			this->height = a.height;
			return *this;
		}

		AspectRatio operator()(const AspectRatio& a) noexcept { return *this = a; }
	};
}