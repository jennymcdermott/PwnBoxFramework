#pragma once

#include "pch.h"
#include <memory>

namespace PwnBoxFramework
{
#if PWNBOX_EXTERNAL_HACK
	class PatternScanner
	{
	private:
		void* m_ProcHandle = nullptr;
	public:
		PatternScanner() = default;
		PatternScanner(void* prochandle) : m_ProcHandle(prochandle) {}
		void Destroy();

		PatternScanner operator()(void* prochandle) { return operator=(PatternScanner(prochandle)); }

		void* ScanForPatternEX(uintptr_t begin, uintptr_t end, char* pattern, char* mask);

	private:

		void* ScanForPattern(char* base, size_t size, char* pattern, char* mask);
	};
#elif PWNBOX_INTERNAL_HACK
	class PatternScanner
	{
	private:
		void* m_ProcHandle = nullptr;
	public:
		PatternScanner() = default;
		PatternScanner(void* prochandle) : m_ProcHandle(prochandle) {}
		void Destroy();

		PatternScanner operator()(void* prochandle) { return operator=(PatternScanner(prochandle)); }
		void* ScanForPattern(char* base, size_t size, char* pattern, char* mask);
	};
#endif
}