#include "Injector.h"
#include "LoaderData.h"
#include <algorithm>
#include <WtsApi32.h>

#if PWNBOX_DLL_INJECTOR
static DWORD __stdcall Stub() { return 0; }
namespace PwnBoxFramework
{
	DWORD Injector::FindProcessId(std::string processName)
	{
		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);
		HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (processSnapshot == INVALID_HANDLE_VALUE) return 0;
		Process32First(processSnapshot, &processInfo);
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
		while (Process32Next(processSnapshot, &processInfo))
		{
			if (!processName.compare(processInfo.szExeFile))
			{
				CloseHandle(processSnapshot);
				return processInfo.th32ProcessID;
			}
		}
		CloseHandle(processSnapshot);
		return 0;
	}

	DWORD __stdcall Injector::LibraryLoader(LPVOID Memory)
	{

		loaderdata* LoaderParams = (loaderdata*)Memory;

		PIMAGE_BASE_RELOCATION pIBR = LoaderParams->BaseReloc;

		DWORD delta = (DWORD)((LPBYTE)LoaderParams->ImageBase - LoaderParams->NtHeaders->OptionalHeader.ImageBase); // Calculate the delta

		while (pIBR->VirtualAddress)
		{
			if (pIBR->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
			{
				int count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
				PWORD list = (PWORD)(pIBR + 1);
				for (int i = 0; i < count; i++)
				{
					if (list[i])
					{
						PDWORD ptr = (PDWORD)((LPBYTE)LoaderParams->ImageBase + (pIBR->VirtualAddress + (list[i] & 0xFFF)));
						*ptr += delta;
					}
				}
			}
			pIBR = (PIMAGE_BASE_RELOCATION)((LPBYTE)pIBR + pIBR->SizeOfBlock);
		}
		PIMAGE_IMPORT_DESCRIPTOR pIID = LoaderParams->ImportDirectory;

		// Resolve DLL imports
		while (pIID->Characteristics)
		{
			PIMAGE_THUNK_DATA OrigFirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)LoaderParams->ImageBase + pIID->OriginalFirstThunk);
			PIMAGE_THUNK_DATA FirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)LoaderParams->ImageBase + pIID->FirstThunk);
			HMODULE hModule = LoaderParams->fnLoadLibraryA((LPCSTR)LoaderParams->ImageBase + pIID->Name);
			if (!hModule) return FALSE;
			while (OrigFirstThunk->u1.AddressOfData)
			{
				if (OrigFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
				{
					// Import by ordinal
					DWORD Function = (DWORD)LoaderParams->fnGetProcAddress(hModule,
						(LPCSTR)(OrigFirstThunk->u1.Ordinal & 0xFFFF));

					if (!Function)
						return FALSE;

					FirstThunk->u1.Function = Function;
				}
				else
				{
					// Import by name
					PIMAGE_IMPORT_BY_NAME pIBN = (PIMAGE_IMPORT_BY_NAME)((LPBYTE)LoaderParams->ImageBase + OrigFirstThunk->u1.AddressOfData);
					DWORD Function = (DWORD)LoaderParams->fnGetProcAddress(hModule, (LPCSTR)pIBN->Name);
					if (!Function)
						return FALSE;

					FirstThunk->u1.Function = Function;
				}
				OrigFirstThunk++;
				FirstThunk++;
			}
			pIID++;
		}

		if (LoaderParams->NtHeaders->OptionalHeader.AddressOfEntryPoint)
		{
			dllmain EntryPoint = (dllmain)((LPBYTE)LoaderParams->ImageBase + LoaderParams->NtHeaders->OptionalHeader.AddressOfEntryPoint);
			//RtlZeroMemory(EntryPoint, 32);
			return EntryPoint((HMODULE)LoaderParams->ImageBase, DLL_PROCESS_ATTACH, NULL); // Call the entry point
		}
		return TRUE;
	}

	void Injector::FindProcessList() noexcept
	{
		WTS_PROCESS_INFO* pWPIs = nullptr;
		DWORD dwProcCount = 0;
		std::vector<std::string> procList;
		if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
		{
			//Go through all processes retrieved
			for (DWORD i = 0; i < dwProcCount; i++)
			{
				//pWPIs[i].pProcessName = process file name only, no path!
				std::string procName = pWPIs[i].pProcessName;
				m_ProcList.push_back(pWPIs[i].pProcessName);
			}
		}

		//Free memory
		if (pWPIs)
		{
			WTSFreeMemory(pWPIs);
			pWPIs = NULL;
		}
		RemoveRepeatedProcesses();
	}

	void Injector::RemoveRepeatedProcesses() noexcept
	{
		std::sort(m_ProcList.begin(), m_ProcList.end());
		m_ProcList.erase(std::unique(m_ProcList.begin(), m_ProcList.end()), m_ProcList.end());
	}

	Injector::Injector() noexcept { FindProcessList(); }
	Injector::~Injector() noexcept { m_ProcList.clear(); }
	DWORD __stdcall Injector::InjectDll(std::string dllname, std::string procname)
	{
		LPCSTR Dll = dllname.c_str(); // Target Dll
		DWORD ProcessId = FindProcessId(procname.c_str());
		loaderdata LoaderParams;
		HANDLE hFile = CreateFileA(Dll, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); // Open the DLL
		DWORD FileSize = GetFileSize(hFile, NULL);
		PVOID FileBuffer = VirtualAlloc(NULL, FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		// Read the DLL
		ReadFile(hFile, FileBuffer, FileSize, NULL, NULL);
		// Target Dll's DOS Header
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
		// Target Dll's NT Headers
		PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)FileBuffer + pDosHeader->e_lfanew);

		// Opening target process.
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
		// Allocating memory for the DLL
		PVOID ExecutableImage = VirtualAllocEx(hProcess, NULL, pNtHeaders->OptionalHeader.SizeOfImage,
			MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		// Copy the headers to target process
		WriteProcessMemory(hProcess, ExecutableImage, FileBuffer,
			pNtHeaders->OptionalHeader.SizeOfHeaders, NULL);

		// Target Dll's Section Header
		PIMAGE_SECTION_HEADER pSectHeader = (PIMAGE_SECTION_HEADER)(pNtHeaders + 1);
		// Copying sections of the dll to the target process
		for (int i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++)
		{
			WriteProcessMemory(hProcess, (PVOID)((LPBYTE)ExecutableImage + pSectHeader[i].VirtualAddress),
				(PVOID)((LPBYTE)FileBuffer + pSectHeader[i].PointerToRawData), pSectHeader[i].SizeOfRawData, NULL);
		}

		// Allocating memory for the loader code.
		PVOID LoaderMemory = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE); // Allocate memory for the loader code
		LoaderParams.ImageBase = ExecutableImage;
		LoaderParams.NtHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)ExecutableImage + pDosHeader->e_lfanew);
		LoaderParams.BaseReloc = (PIMAGE_BASE_RELOCATION)((LPBYTE)ExecutableImage
			+ pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		LoaderParams.ImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)ExecutableImage
			+ pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		LoaderParams.fnLoadLibraryA = LoadLibraryA;
		LoaderParams.fnGetProcAddress = GetProcAddress;
		// Write the loader information to target process
		WriteProcessMemory(hProcess, LoaderMemory, &LoaderParams, sizeof(loaderdata), NULL);


		// Write the loader code to target process
		WriteProcessMemory(hProcess, (PVOID)((loaderdata*)LoaderMemory + 1), LibraryLoader,
			(DWORD)Stub - (DWORD)LibraryLoader, NULL);
		// Create a remote thread to execute the loader code
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((loaderdata*)LoaderMemory + 1), LoaderMemory, 0, NULL);

		// Wait for the loader to finish executing
		if (hThread != nullptr && hThread != INVALID_HANDLE_VALUE)
			CloseHandle(hThread);
		return 0;
	}
	const std::vector<std::string>& Injector::GetProcessList() noexcept { return m_ProcList; }
	void Injector::RefreshProcessList() noexcept { FindProcessList(); }
}
#endif