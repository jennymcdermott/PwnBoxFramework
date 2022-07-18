#pragma once

#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class InjectorGUI
	{
	public:
		void OnImGuiRender();
	private:
		void renderInjectorWindow();
	private:
		struct InjectorData
		{
			std::string targetProc;
			std::string targetDll;
			std::string userSearch;
		};
		InjectorData m_Data;
	private:
		std::vector<const char*> m_ProcList; // For some reason, I fucking need this...
	};
}