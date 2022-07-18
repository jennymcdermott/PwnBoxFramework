#pragma once
#include "pch.h"
#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
#include <optional>
#include "WindowProps.h"
#include "PwnBoxFramework/Renderer/Renderer.h"

namespace PwnBoxFramework
{
	class Window
	{
	private:
		class WindowClass
		{
		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass() noexcept;
			WindowClass(const WindowClass&) = delete;
			static WindowClass s_wndClass;
			static constexpr const char* s_wndClassName = "MYTESTWINDOW";
			HINSTANCE m_hInst;
		};
	public:
		Window(const WindowProps& props = WindowProps()) noexcept;
		static Scope<Window> Create(const WindowProps& props);

		static std::optional<int> ProcessMessages();
		int GetMessages() noexcept;
		void OnUpdate() noexcept;
		void CloseWindow() noexcept;
		HWND& GetWindowNative() noexcept;
		const bool& IsWindowClosed() const noexcept { return m_WindowClosed; }
		const int& getWidth() const noexcept;
		const int& getHeight() const noexcept;
		const bool& isFullScreen() const noexcept;
		~Window() noexcept;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		bool m_WindowClosed;
		HWND m_hWnd;
		MSG m_Msg;
		WindowProps m_Props;
	};
}
#endif