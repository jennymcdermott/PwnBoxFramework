#pragma once

#include "PwnBoxFramework/Layer/Layer.h"
#include "PwnBoxFramework/Renderer/Renderer.h"

#if PWNBOX_EXTERNAL_HACK or PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	class PwnBoxUI : public Layer
	{
	public:
		PwnBoxUI() noexcept;
		void OnAttach() override {}
		void OnDetach() override {}
		void OnUpdate() override {}
		void Begin() noexcept;
		void End() noexcept;
	};
}
#endif