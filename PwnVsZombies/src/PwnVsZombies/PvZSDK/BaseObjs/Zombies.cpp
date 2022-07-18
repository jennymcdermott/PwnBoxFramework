#include "Zombies.h"
#include <vector>
#include <Windows.h>

// All zombies on field are seperated by 0x168
namespace PwnBoxFramework
{
	zombies::zombies(const int& gameModule) noexcept :
		m_gameModule(gameModule) {
		init();
	}

	int zombies::getZombieType() noexcept
	{
		return 0;
	}

	int zombies::getInterface() noexcept { return m_interface; }
	void zombies::init() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x868 });
		m_zMemory.baseZombieAddr = memory->GetPointerAddress(m_interface, { 0xA8 });
		m_zMemory.zombieHealthAddr = memory->GetPointerAddress(m_zMemory.baseZombieAddr, { 0xC8 });
		m_zMemory.zombieIdAddr = memory->GetPointerAddress(m_zMemory.baseZombieAddr, { 0x24 });
		m_zMemory.baseRTimerAddr = memory->GetPointerAddress(m_interface, { 0x55B4 });
	}

	int zombies::getRefreshTimer() noexcept { return memory->ReadProcMemory<int>(m_zMemory.baseRTimerAddr); }

	void zombies::OnUpdate() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x868 });
		m_zMemory.baseZombieAddr = memory->GetPointerAddress(m_interface, { 0xC0 });
		m_zMemory.zombieHealthAddr = memory->GetPointerAddress(m_interface, { 0xC8 });
		m_zMemory.baseRTimerAddr = memory->GetPointerAddress(m_interface, { 0x55B4 });
	}

}