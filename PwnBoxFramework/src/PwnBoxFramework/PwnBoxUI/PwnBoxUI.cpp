#include "PwnBoxUI.h"

#if PWNBOX_EXTERNAL_HACK or PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
    PwnBoxUI::PwnBoxUI() noexcept : Layer("PwnBoxUI Layer")
    {
    }

    void PwnBoxUI::Begin() noexcept { renderer->BeginGuiRender(); }
    void PwnBoxUI::End() noexcept { renderer->EndGuiRender(); }
}
#endif