#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <memory>
/*
* This is a wrapper class for a dll injector.
* Why is this here?
* It's so that way PwnBox hacks have
* a way of being injected that is easy and quick. If you
* want to add more or change the injection method, you can.
*/
#if PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	class Injector
	{
	private:
		std::vector<std::string> m_ProcList;
		DWORD FindProcessId(std::string processName);
		static DWORD __stdcall LibraryLoader(LPVOID Memory);
		void FindProcessList() noexcept;
		void RemoveRepeatedProcesses() noexcept;
	public:
		Injector() noexcept;
		virtual ~Injector() noexcept;
		DWORD __stdcall InjectDll(std::string dllname, std::string procname);
		const std::vector<std::string>& GetProcessList() noexcept;
		void RefreshProcessList() noexcept;
	};
	inline std::unique_ptr<PwnBoxFramework::Injector> injector;
}
#endif