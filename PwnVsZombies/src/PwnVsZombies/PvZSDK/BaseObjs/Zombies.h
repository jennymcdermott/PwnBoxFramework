#pragma once


#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class zombies
	{
	public:
		int getInterface() noexcept;
	private:
		void init() noexcept;
	public:
		zombies() noexcept = default;
		zombies(const int& gameModule) noexcept;
		int getZombieType() noexcept;
		int getZombieHealth()   noexcept; //{ return m_gameModule + 0x141CDA; }
		int getZombieShield()   noexcept; //{ return m_gameModule + 0x141651; }
		int getZombieHeadgear() noexcept; //{ return m_gameModule + 0x1419FA; }
		int getRefreshTimer() noexcept;
		void OnUpdate() noexcept;
	private:
		struct ZombieMemory
		{
			int baseGameAddr = 0;
			int baseLevelAddr = 0;
			int zombieIdAddr = 0;
			int baseZombieAddr = 0;
			int zombieHealthAddr = 0;
			int zombieShieldHpAddr = 0;
			int zombieHeadgearHpAddr = 0;
			int baseRTimerAddr = 0;
		};
		ZombieMemory m_zMemory;
	private:
		int m_interface = 0;
		int m_gameModule = 0;
	};
}