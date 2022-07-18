#pragma once

#include "PwnBoxFramework/Renderer/Renderer.h"
#include <d3d11.h>

#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	class ImGuiRenderer : public Renderer
	{
	public:
		ImGuiRenderer() noexcept;
		virtual ~ImGuiRenderer() noexcept;
		void RenderFrame() noexcept override;
		void SetRenderTarget() noexcept override;
		void ClearBuffer(const Color& color) noexcept override;
		void BeginGuiRender() noexcept override;
		void EndGuiRender() noexcept override;
		void ResizeBuffer() noexcept override;
		void GetWindowSize(const AspectRatio& wnd) noexcept override;
	private:
		void InitImGuiRenderer(HWND hWnd) noexcept override;
		void DestroyImGuiRenderer() noexcept override;
		void Init(HWND hWnd) noexcept override;
		void Destroy() noexcept override;
		void SetStyleColors() noexcept override;
	private:
		HWND m_hWnd;
		ID3D11Device* m_pDevice = nullptr;
		IDXGISwapChain* m_pSwap = nullptr;
		ID3D11DeviceContext* m_pContext = nullptr;
		ID3D11Resource* m_pBackBuffer = nullptr;
		D3D11_VIEWPORT m_ViewPort = {};
		ID3D11RenderTargetView* m_pTarget = nullptr;
		AspectRatio m_WndSize;
	};
}
#endif