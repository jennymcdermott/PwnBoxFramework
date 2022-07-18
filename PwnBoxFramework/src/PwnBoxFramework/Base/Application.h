#pragma once

#include "Base.h"
#include <Windows.h>
#include "PwnBoxFramework/Window/Window.h"
#include "PwnBoxFramework/Layer/LayerStack.h"
#include "PwnBoxFramework/Window/WindowProps.h"
#include "PwnBoxFramework/PwnBoxUI/PwnBoxUI.h"

namespace PwnBoxFramework 
{
#if PWNBOX_INTERNAL_HACK
	class Application
	{
	private:
		static Application* s_Instance;
	public:
		Application() noexcept;
		virtual ~Application() noexcept;

		void PushLayer(Layer* layer) noexcept;
		void PushOverlay(Layer* overlay) noexcept;

		static Application& GetApp() noexcept;
		void Close() noexcept;
		void run() noexcept;
	private:
		LayerStack m_LayerStack;
		bool m_iRunning = true;
	};
#endif
#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
	class Application
	{
	private:
		static Application* s_Instance;
	public:
		Application(const WindowProps& props = WindowProps()) noexcept;
		virtual ~Application() noexcept;

		void PushLayer(Layer* layer) noexcept;
		void PushOverlay(Layer* overlay) noexcept;
		Window& GetWindow() noexcept;
		static Application& GetApp() noexcept;
		void Close() noexcept;
		void run() noexcept;
	private:
		LayerStack m_LayerStack;
		PwnBoxUI* m_PwnBoxUI;
		bool m_iRunning = true;
		Scope<Window> m_Window;
	};
#endif

	Application* CreateApp() noexcept;
}