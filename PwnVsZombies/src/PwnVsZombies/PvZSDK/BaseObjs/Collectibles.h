#pragma once

#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class collectibles
	{
	private:
		void init() noexcept;
	public:
		collectibles() noexcept = default;
		collectibles(int gameModule) noexcept;

		int getTotalCollectibles() noexcept;
		int getMaxCollectibles() noexcept;
		int getCurrentCount() noexcept;
		int getspawnTimer() noexcept;
		int getdespawnTimer() noexcept;
		float getX() noexcept;
		float getY() noexcept;
		bool isClicked() noexcept;
	private:
		struct CollectibleMem
		{
			int collectiblesBaseAddr;
			int collectiblesCurrentCount;
			int collectiblesTotalCount;
			int collectiblesSpawnTimer;
			int collectiblesDespawnTimer;
			int collectiblesXPosAddr;
			int collectiblesYPosAddr;
			int collectiblesIsClickedAddr;
		};
		CollectibleMem m_cMemory;
	private:
		int m_interface = 0;
		int m_gameModule = 0;
	};
}