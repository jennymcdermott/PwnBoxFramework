#include "Renderer.h"
#include "PwnBoxFramework/Renderer/DirectX11/ImGuiRenderer.h"

#if PWNBOX_EXTERNAL_HACK || PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{	 
	Scope<Renderer> Renderer::Create() noexcept { return CreateScope<ImGuiRenderer>(); }
}
#endif