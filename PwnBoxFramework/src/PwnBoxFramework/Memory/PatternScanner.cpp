#include "PatternScanner.h"
#if !PWNBOX_DLL_INJECTOR
namespace PwnBoxFramework
{
	void PatternScanner::Destroy() { m_ProcHandle = nullptr; }

#if PWNBOX_EXTERNAL_HACK
	void* PatternScanner::ScanForPattern(char* base, size_t size, char* pattern, char* mask)
	{
		size_t patternLength = strlen(pattern);
		for (unsigned int i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (unsigned int j = 0; j < patternLength; j++)
			{
				if (mask[j] != '?' && pattern[j] != *(base + i + j))
				{
					found = false;
					break;
				}
				if (found)
					return (void*)(base + i);
			}
		}
		return nullptr;
	}

	void* PatternScanner::ScanForPatternEX(uintptr_t begin, uintptr_t end, char* pattern, char* mask)
	{
		uintptr_t currentChunk = begin;
		SIZE_T bytesRead;
		while (currentChunk < end)
		{
			char buffer[4096];
			DWORD oldProtect;
			VirtualProtectEx(m_ProcHandle, (void*)currentChunk, sizeof(buffer), PROCESS_ALL_ACCESS, &oldProtect);
			ReadProcessMemory(m_ProcHandle, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
			VirtualProtectEx(m_ProcHandle, (void*)currentChunk, sizeof(buffer), oldProtect, nullptr);

			if (bytesRead == 0)
				return nullptr;
			void* InternalAddress = ScanForPattern((char*)buffer, bytesRead, pattern, mask);
			if (InternalAddress != nullptr)
			{
				uintptr_t offsetFromBuffer = (uintptr_t)InternalAddress - (uintptr_t)&buffer;
				return (void*)(currentChunk + offsetFromBuffer);
			}
			else {
				currentChunk = currentChunk + bytesRead;
			}
		}
		return nullptr;
	}

#else
	void* PatternScanner::ScanForPattern(char* base, size_t size, char* pattern, char* mask)
	{
		size_t patternLength = strlen(pattern);
		for (unsigned int i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (unsigned int j = 0; j < patternLength; j++)
			{
				if (mask[j] != '?' && pattern[j] != *(base + i + j))
				{
					found = false;
					break;
				}
				if (found)
					return (void*)(base + i);
			}
		}
		return nullptr;
	}
#endif
}
#endif