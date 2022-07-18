#include "LevelManager.h"
#include <vector>
#include <Windows.h>

namespace PwnBoxFramework
{

	void levelManager::init(const int& gameModule) noexcept
	{
		m_gameModule = gameModule;
		std::cout << "void levelManger::init(const int& gameModule) noexcept \n{\n\tm_gameModule = "
			<< std::hex << gameModule << std::dec << "\n";
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x714, 0x0, 0x154 });
		m_lmMemory.levelMgrSeedAddr = memory->GetPointerAddress(m_interface, { 0x150, 0x15C });
		m_lmMemory.levelMgrSunAddr = memory->GetPointerAddress(m_interface, { 0x5578 });
	}

	void levelManager::OnUpdate() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x714, 0x0, 0x154 });
		m_lmMemory.levelMgrSeedAddr = memory->GetPointerAddress(m_interface, { 0x150, 0x15C });
		m_lmMemory.levelMgrSunAddr = memory->GetPointerAddress(m_interface, { 0x5578 });
	}

	int levelManager::getLevelInterface() noexcept { return m_interface; }

	int levelManager::getSeedSelection(int index) noexcept
	{
		std::vector<DWORD> SeedOffsets{ 0x5C };
		for (int i = 0; i < index; i++) SeedOffsets[0] += 0x50;
		return memory->ReadProcMemory<int>(m_lmMemory.levelMgrSeedAddr, SeedOffsets);
	}

	int levelManager::getSunCount() noexcept { return memory->ReadProcMemory<int>(m_lmMemory.levelMgrSunAddr); }
	void levelManager::setSunCount(int value) noexcept { memory->WriteProcMemory<int>(m_lmMemory.levelMgrSunAddr, value); }
}