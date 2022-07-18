#include "Memory.h"

#if !PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
#if PWNBOX_INTERNAL_HACK
	Memory::Memory(HMODULE modHandle) noexcept
		: moduleHandle(modHandle) { }

	extern "C" DWORD WINAPI _CRT_INIT(HMODULE moduleHandle, DWORD reason, LPVOID reserved);
	static DWORD WINAPI unload(HMODULE moduleHandle) noexcept
	{
		_CRT_INIT(moduleHandle, DLL_PROCESS_DETACH, nullptr);
		FreeLibraryAndExitThread(moduleHandle, 0);
	}

	void Memory::uninstall()
	{
		if (HANDLE thread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(unload), moduleHandle, 0, nullptr))
			CloseHandle(thread);
	}

	void* Memory::PatternScanModule(const std::string_view mod, char* pattern, char* mask)
	{
		MODULEENTRY32 modEntry = GetModuleInfo(mod);
		return m_Scanner.ScanForPattern((char*)modEntry.modBaseAddr, modEntry.dwSize, pattern, mask);
	}

#else
	void* Memory::PatternScanModule(const std::string_view mod, char* pattern, char* mask)
	{
		MODULEENTRY32 modEntry = GetModuleInfo(mod);
		uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
		uintptr_t end = begin + modEntry.dwSize;
		return m_Scanner.ScanForPatternEX(begin, end, pattern, mask);
	}
#endif


	void Memory::HookProcess(const std::string_view procName)
	{
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(snapShot, &entry))
		{
			if (!procName.compare(entry.szExeFile))
			{
				m_ProcessId = entry.th32ProcessID;
				procHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_ProcessId);
				break;
			}
		}

		// Free handle
		if (snapShot)
			::CloseHandle(snapShot);
		m_Scanner(procHandle);
	}

	void Memory::Destroy()
	{
		if (procHandle)
			::CloseHandle(procHandle);
		m_Scanner.Destroy();
	}

	MODULEENTRY32 Memory::GetModuleInfo(const std::string_view moduleName)
	{
		MODULEENTRY32 modEntry = { 0 };
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_ProcessId);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			modEntry.dwSize = sizeof(MODULEENTRY32);
			while (::Module32Next(hSnapshot, &modEntry))
			{
				if (!moduleName.compare(modEntry.szModule)) break;
			}
			CloseHandle(hSnapshot);
		}
		return modEntry;
	}

	const uintptr_t Memory::GetModuleAddress(const std::string_view moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_ProcessId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapShot, &entry))
		{
			if (!moduleName.compare(entry.szModule))
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}

	uintptr_t Memory::GetProcessId() noexcept { return m_ProcessId; }

}
#endif