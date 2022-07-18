#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
#include "Window.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include "resource.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
namespace PwnBoxFramework
{
	Window::WindowClass Window::WindowClass::s_wndClass;
	const char* Window::WindowClass::GetName() noexcept { return s_wndClassName; }
	HINSTANCE Window::WindowClass::GetInstance() noexcept { return s_wndClass.m_hInst; }
	Window::WindowClass::WindowClass() noexcept
		: m_hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc{ 0 };
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = static_cast<HICON>(LoadImage(m_hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,0));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass() noexcept { UnregisterClass(s_wndClassName, GetInstance()); }
	Scope<Window> Window::Create(const WindowProps& props) { return CreateScope<Window>(props); }
	Window::Window(const WindowProps& props) noexcept : m_Props(props)
	{
		ZeroMemory(&m_hWnd, sizeof(HWND));
		RECT rc = { 0 };
		rc.left = 100;
		rc.right = m_Props.WindowSize.width + rc.left;
		rc.top = 100;
		rc.bottom = m_Props.WindowSize.height + rc.top;
		AdjustWindowRect(&rc, WS_CAPTION | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		m_hWnd = CreateWindow(WindowClass::GetName(), m_Props.Title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
			(rc.right - rc.left), (rc.bottom - rc.top),
			nullptr, nullptr, WindowClass::GetInstance(), this); // 6th argument is parent window

		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(m_hWnd);

		m_WindowClosed = false;
		m_Msg = MSG{ 0 };
		renderer = Renderer::Create();
		renderer->GetWindowSize(m_Props.WindowSize);
		renderer->Init(m_hWnd);
		renderer->InitImGuiRenderer(m_hWnd);
	}
	Window::~Window() noexcept { DestroyWindow(m_hWnd); }

	int Window::GetMessages() noexcept { return m_Msg.message; }
	void Window::OnUpdate() noexcept { UpdateWindow(m_hWnd); }

	void Window::CloseWindow() noexcept
	{
		m_WindowClosed = true;
		SendMessageA(m_hWnd, WM_CLOSE, 0, 0);
	}

	HWND& Window::GetWindowNative() noexcept { return m_hWnd; }

	const int& Window::getWidth() const noexcept { return m_Props.WindowSize.width; }
	const int& Window::getHeight() const noexcept { return m_Props.WindowSize.height; }
	const bool& Window::isFullScreen() const noexcept { return m_Props.isWindowed; }

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg;
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return {};
	}

	LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

			// Set message proc to normal (non-setup) handler now that setup is finished.
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hWnd));

			// forward message to window class handler
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, message, wParam, lParam);
		}
		// if we get a msg before the WM_NCCREATE message, it will be handled with default handler
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, message, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;
		switch (message)
		{
		case WM_QUIT:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}
		case WM_SIZING:
		{
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
			renderer->ResizeBuffer();
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		case WM_GETMINMAXINFO:
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}
#endif