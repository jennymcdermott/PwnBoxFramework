#pragma once

#include <PwnBoxFramework.h>
#include "PwnBoxInjector/InjectorUI/InjectorUI.h"

namespace PwnBoxFramework
{
	class AppLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
	private:
		InjectorGUI m_Gui;
	};
}