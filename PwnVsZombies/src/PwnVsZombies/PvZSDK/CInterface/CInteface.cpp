#include "CInteface.h"
#include <vector>
#include <Windows.h>
#include <iostream>
#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{

	void CInterface::init() noexcept
	{
		m_iMemory.gameInterface = memory->GetPointerAddress(m_gameModule, { 0x95C });
		m_iMemory.gameState = memory->GetPointerAddress(m_iMemory.gameInterface, { 0x8 });
		m_iMemory.isPaused = memory->GetPointerAddress(m_iMemory.gameInterface, { 0x40 });
	}

	CInterface::CInterface(const int& gameModule) noexcept : m_gameModule(gameModule) { init(); }
	int CInterface::getInterface() noexcept { return m_iMemory.gameInterface; }
	bool CInterface::isInLevel() noexcept { return m_iMemory.gameState == 6 ? true : false; }
	bool CInterface::isPaused() noexcept { return m_iMemory.isPaused == 1 ? true : false; }
}