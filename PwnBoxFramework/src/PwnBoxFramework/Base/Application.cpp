#include "Application.h"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace PwnBoxFramework
{
#if PWNBOX_INTERNAL_HACK
	Application* Application::s_Instance = nullptr;
	Application::Application() noexcept
	{
		s_Instance = this;
	}

	void Application::PushLayer(Layer* layer) noexcept
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) noexcept
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application() noexcept { s_Instance = nullptr; }

	Application& Application::GetApp() noexcept { return *s_Instance; }

	void Application::Close() noexcept { m_iRunning = false; }

	void Application::run() noexcept
	{
		while (m_iRunning)
		{
			for (auto layers : m_LayerStack)
				layers->OnUpdate();
		}
	}
#else
	Application* Application::s_Instance = nullptr;
	Application::Application(const WindowProps& props) noexcept
	{
		s_Instance = this;
		m_Window = Window::Create(props);
		m_PwnBoxUI = new PwnBoxUI();
		PushOverlay(m_PwnBoxUI);
	}

	void Application::PushLayer(Layer* layer) noexcept
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) noexcept
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Window& Application::GetWindow() noexcept { return *m_Window; }

	Application::~Application() noexcept 
	{
		s_Instance = nullptr;
	}

	Application& Application::GetApp() noexcept { return *s_Instance; }

	void Application::Close() noexcept { m_iRunning = false; }

	void Application::run() noexcept
	{
		while (m_iRunning)
		{
			if (const auto ecode = Window::ProcessMessages())
				Close();

			for (auto layers : m_LayerStack)
				layers->OnUpdate();

			renderer->ClearBuffer({ 0.0f,0.0f,0.0f,1.0f });
			m_PwnBoxUI->Begin();
			for (auto layers : m_LayerStack)
				layers->OnImGuiRender();
			m_PwnBoxUI->End();
			renderer->RenderFrame();
		}
	}
#endif
}