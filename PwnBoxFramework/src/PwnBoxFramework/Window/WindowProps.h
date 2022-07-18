#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "AspectInfo.h"

namespace PwnBoxFramework
{
	struct WindowProps
	{
		std::string Title;
		AspectRatio WindowSize;
		bool isWindowed;

		WindowProps(const std::string& title = "ReCore Engine: TestApp", const AspectRatio& windowSize = AspectRatio({ 1280, 720 }), bool fullscreen = false)
			: Title(title), WindowSize(windowSize), isWindowed(fullscreen) { }

		WindowProps operator()(const WindowProps& p) { return operator=(p); }
	};
}