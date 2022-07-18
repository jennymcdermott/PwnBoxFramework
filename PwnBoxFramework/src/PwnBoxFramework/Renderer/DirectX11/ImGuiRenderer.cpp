#include "ImGuiRenderer.h"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <iostream>

#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	ImGuiRenderer::ImGuiRenderer() noexcept
		: Renderer() { }
	
	ImGuiRenderer::~ImGuiRenderer() noexcept 
	{
		DestroyImGuiRenderer();
		Destroy();
	}

	void ImGuiRenderer::BeginGuiRender() noexcept
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndGuiRender() noexcept
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Render();
		m_pContext->OMSetRenderTargets(1, &m_pTarget, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiRenderer::ResizeBuffer() noexcept
	{
		m_pContext->OMSetRenderTargets(1, nullptr, NULL);
		m_pTarget->Release();
		m_pContext->Flush();
		m_pSwap->ResizeBuffers(1, m_WndSize.width, m_WndSize.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
		m_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&m_pBackBuffer));
		m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pTarget);
		m_pBackBuffer->Release();

		m_ViewPort.Width = m_WndSize.width;
		m_ViewPort.Height = m_WndSize.height;
		m_ViewPort.MinDepth = 0.0f;
		m_ViewPort.MaxDepth = 1.0f;
		m_ViewPort.TopLeftX = 0;
		m_ViewPort.TopLeftY = 0;
		m_pContext->RSSetViewports(1, &m_ViewPort);
	}

	void ImGuiRenderer::GetWindowSize(const AspectRatio& wnd) noexcept { m_WndSize(wnd); }

	void ImGuiRenderer::InitImGuiRenderer(HWND hWnd) noexcept
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Window
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		
		SetStyleColors();
		
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	}

	void ImGuiRenderer::RenderFrame() noexcept { m_pSwap->Present(0u, 0u); }

	void ImGuiRenderer::SetRenderTarget() noexcept
	{
		m_pContext->OMSetRenderTargets(1, &m_pTarget, NULL);
	}

	void ImGuiRenderer::ClearBuffer(const Color& color) noexcept
	{
		const float colorValues[] = { color.red,color.green,color.blue,color.alpha };
		m_pContext->ClearRenderTargetView(m_pTarget, colorValues);
	}

	void ImGuiRenderer::Init(HWND hWnd) noexcept
	{
		m_hWnd = hWnd;
		DXGI_SWAP_CHAIN_DESC sd = {};

		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, nullptr, 0, D3D11_SDK_VERSION, &sd, &m_pSwap, &m_pDevice, nullptr, &m_pContext);

		m_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&m_pBackBuffer));
		m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pTarget);
		m_pBackBuffer->Release();

		m_ViewPort.Width = m_WndSize.width;
		m_ViewPort.Height = m_WndSize.height;
		m_ViewPort.MinDepth = 0.0f;
		m_ViewPort.MaxDepth = 1.0f;
		m_ViewPort.TopLeftX = 0;
		m_ViewPort.TopLeftY = 0;
		m_pContext->RSSetViewports(1, &m_ViewPort);
	}

	void ImGuiRenderer::Destroy() noexcept
	{
		if (m_pTarget != nullptr)
			m_pTarget->Release();

		if (m_pContext != nullptr)
			m_pContext->Release();

		if (m_pSwap != nullptr)
			m_pSwap->Release();

		if (m_pDevice != nullptr)
			m_pDevice->Release();
	}

	void ImGuiRenderer::SetStyleColors() noexcept
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.85f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.60f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.90f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.85f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 1.00f, 0.00f, 0.00f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 1.00f, 0.00f, 0.00f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.65f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.6f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 1.f, 0.f, 0.f, .85f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.90f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 1.00f, 0.00f, 0.00f, 0.60f };

		// Check Mark
		colors[ImGuiCol_CheckMark] = ImVec4{ 1.0f,0.f , 0.f, 1.0f };

		// Resize Grip
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 0.00f, 0.85f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
		// Sliders
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.00f, 0.85f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
	}

	void ImGuiRenderer::DestroyImGuiRenderer() noexcept
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}
}
#endif