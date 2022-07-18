#pragma once
 
#include "pch.h"
#include <windows.h>
#include "PwnBoxFramework/Base/Color.h"
#include "PwnBoxFramework/Window/AspectInfo.h"
/*
* Note: 
* - Due to the flexibility that this grants, I'd choose this 
* - way of modeling how rendering should work as well as maybe
* - cheat features and stuff.
*/

#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	class Renderer
	{
	public:
		virtual ~Renderer() noexcept {}
		static Scope<Renderer> Create() noexcept;

		virtual void RenderFrame() noexcept = 0;
		virtual void SetRenderTarget() noexcept = 0;
		virtual void ClearBuffer(const Color& color) noexcept = 0;
		virtual void BeginGuiRender() noexcept = 0;
		virtual void EndGuiRender() noexcept = 0;
		virtual void Init(HWND hWnd) noexcept = 0;
		virtual void InitImGuiRenderer(HWND hWnd) noexcept = 0;
		virtual void GetWindowSize(const AspectRatio& wnd) noexcept = 0;
		virtual void ResizeBuffer() noexcept = 0;
	private:
		virtual void DestroyImGuiRenderer() noexcept = 0;
		virtual void SetStyleColors() noexcept = 0;
		virtual void Destroy() noexcept = 0;
	};
	inline Scope<Renderer> renderer;
}
#endif