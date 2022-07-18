#include "AppLayer.h"
#include "PwnBoxInjector/InjectorUI/InjectorUI.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace PwnBoxFramework
{
	void AppLayer::OnAttach()  { injector = std::make_unique<Injector>(); }

	void AppLayer::OnDetach() { }

	void AppLayer::OnUpdate() { }

	void AppLayer::OnImGuiRender() { m_Gui.OnImGuiRender(); }
}