#pragma once

#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class AppLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
	};
}