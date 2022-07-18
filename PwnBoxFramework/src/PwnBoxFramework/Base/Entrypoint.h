#pragma once

#include "Base.h"
#include "Application.h"
#include "PwnBoxFramework/Memory/Memory.h"

#if defined(PWNBOX_PLATFORM_WIN)
#include <Windows.h>
#if (PWNBOX_INTERNAL_HACK)
extern "C" DWORD WINAPI _CRT_INIT(HMODULE moduleHandle, DWORD reason, LPVOID reserved);
extern PwnBoxFramework::Application* PwnBoxFramework::CreateApp() noexcept;

int __stdcall DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        PwnBoxFramework::memory = std::make_unique<PwnBoxFramework::Memory>(hModule);
        auto SandBox = PwnBoxFramework::CreateApp();
        SandBox->run();
        delete SandBox;
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        PwnBoxFramework::memory->uninstall();
        break;
    }
    return 0;  // Successful DLL_PROCESS_ATTACH.
}
#elif PWNBOX_EXTERNAL_HACK  || PWNBOX_DLL_INJECTOR and not defined(PWNBOX_BUILD_DLL)
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	auto SandBox = PwnBoxFramework::CreateApp();
	SandBox->run();
	delete SandBox;
	return 0;
}
#else
	#error Platform not supported, this may be for windows only for a while.
#endif
#endif