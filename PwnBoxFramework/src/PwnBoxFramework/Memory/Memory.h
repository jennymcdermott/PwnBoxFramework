#pragma once

#include <memory>
#include "pch.h"
#include <iostream>
#include "PatternScanner.h"

namespace PwnBoxFramework
{
#if PWNBOX_INTERNAL_HACK
	class Memory
	{
	private:
		uintptr_t m_ProcessId = 0;
		MODULEENTRY32 GetModuleInfo(const std::string_view moduleName);
	public:

		Memory() noexcept = default;
		Memory(HMODULE modHandle) noexcept;
		HMODULE moduleHandle;

		void uninstall();
		void* procHandle = nullptr;
		void HookProcess(const std::string_view procName);
		const uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept;
		uintptr_t GetProcessId() noexcept;
		void Destroy();

		void* PatternScanModule(const std::string_view module, char* pattern, char* mask);

		const int GetPointerAddress(uintptr_t address, std::vector<DWORD> offsets) noexcept
		{
			uintptr_t addr = address;
			for (unsigned int i = 0; i < offsets.size(); i++)
			{
				addr = *(uintptr_t*)addr;
				addr += offsets[i];
			}
			return addr;
		}

		template<typename T>
		constexpr const T ReadProcMemory(uintptr_t address) noexcept { return *(T*)address; }

		template<typename T>
		constexpr const T ReadProcMemory(uintptr_t address, std::vector<DWORD> offsets) noexcept
		{
			uintptr_t addr = address;
			for (unsigned int i = 0; i < offsets.size(); i++)
			{
				addr = *(uintptr_t*)addr;
				addr += offsets[i];
			}
			return *(T*)addr;
		}

		const uintptr_t ReadProcMemory(uintptr_t address, std::vector<DWORD> offsets) noexcept
		{
			uintptr_t addr = address;
			for (unsigned int i = 0; i < offsets.size(); i++)
			{
				addr = *(uintptr_t*)addr;
				addr += offsets[i];
			}
			return *(uintptr_t*)addr;
		}

		template<typename T>
		void WriteProcMemory(uintptr_t address, T& value) noexcept { *(T*)address = value; }

		void PatchMemory(const uintptr_t& address, BYTE* value, unsigned int size) noexcept
		{
			DWORD oldProtect;
			VirtualProtect((BYTE*)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((void*)address, value, size);
			VirtualProtect((BYTE*)address, size, oldProtect, &oldProtect);
		}

		void SetToNOP(void* dst, unsigned int size)
		{
			DWORD oldProtect;
			VirtualProtect((BYTE*)dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
			memset(&dst, 0x90, size);
			VirtualProtect((BYTE*)dst, size, oldProtect, &oldProtect);
		}

		bool HookFunction(void* toHook, void* ourFunc, int len)
		{
			if (len < 5) return false;
			DWORD oldProtect;
			VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &oldProtect);

			DWORD rlAddr = ((DWORD)ourFunc - (DWORD)toHook) - 5;
			*(BYTE*)toHook = 0xE9;
			*(DWORD*)((DWORD)toHook + 1) = rlAddr;

			DWORD tmp;
			VirtualProtect(toHook, len, oldProtect, &tmp);
			return true;
		}

	private:
		PatternScanner m_Scanner;
	};
	inline std::unique_ptr<Memory> memory;
#elif PWNBOX_EXTERNAL_HACK
	class Memory
	{
	private:
		uintptr_t m_ProcessId = 0;
		MODULEENTRY32 GetModuleInfo(const std::string_view moduleName);
	public:
		Memory() noexcept = default;

		void uninstall();
		void* procHandle = nullptr;
		void HookProcess(const std::string_view procName);
		const uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept;
		uintptr_t GetProcessId() noexcept;
		void Destroy();

		void* PatternScanModule(const std::string_view module, char* pattern, char* mask);
		const int GetPointerAddress(uintptr_t address, std::vector<DWORD> offsets) noexcept
		{
			uintptr_t addr = address;
			for (unsigned int i = 0; i < offsets.size(); i++)
			{
				::ReadProcessMemory(procHandle, (BYTE*)(addr), &addr, sizeof(addr), nullptr);
				addr += offsets[i];
			}
			return addr;
		}

		template<typename T>
		constexpr const T ReadProcMemory(uintptr_t address) noexcept
		{
			T value = { };
			::ReadProcessMemory(procHandle, reinterpret_cast<const void*>(address), &value, sizeof(value), nullptr);
			return value;
		}

		template<typename T>
		const T ReadProcMemory(uintptr_t address, std::vector<DWORD> offsets) noexcept
		{
			uintptr_t addr = address;
			for (unsigned int i = 0; i < offsets.size(); i++)
			{
				::ReadProcessMemory(procHandle, (BYTE*)(addr), &addr, sizeof(addr), nullptr);
				addr += offsets[i];
			}
			T value = {};
			::ReadProcessMemory(procHandle, (BYTE*)addr, &value, sizeof(value), nullptr);
			return value;
		}

		template<typename T>
		constexpr void WriteProcMemory(const uintptr_t& address, const T& value) noexcept
		{
			::WriteProcessMemory(procHandle, (BYTE*)(address), &value, sizeof(value), nullptr);
		}

		template<typename T>
		constexpr void WriteProcMemory(const uintptr_t& address, const T& value, int size) noexcept
		{
			::WriteProcessMemory(procHandle, (BYTE*)(address), &value, sizeof(size), nullptr);
		}

		void PatchMemory(void* address, BYTE* value, unsigned int size) noexcept
		{
			DWORD oldProtect;
			::VirtualProtectEx(procHandle, address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
			::WriteProcessMemory(procHandle, address, &value, size, nullptr);
			::VirtualProtectEx(procHandle, address, size, oldProtect, &oldProtect);
		}

		void SetToNOP(void* dst, unsigned int size) noexcept
		{
			BYTE* nopArray = new BYTE[size];
			memset(nopArray, 0x90, size);
			DWORD oldProtect;
			::VirtualProtectEx(procHandle, dst, size, PAGE_READWRITE, &oldProtect);
			::WriteProcessMemory(procHandle, dst, nopArray, size, nullptr);
			::VirtualProtectEx(procHandle, dst, size, oldProtect, &oldProtect);
			delete[] nopArray;
		}
	private:
		PatternScanner m_Scanner;
	};
	inline std::unique_ptr<Memory> memory;
#endif
}