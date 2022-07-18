#pragma once

#include <memory>
#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class levelManager
	{
	public:
		levelManager() noexcept = default;
		void init(const int& gameModule) noexcept;

		int getLevelInterface() noexcept;
		void OnUpdate() noexcept;
		int getSeedSelection(int index) noexcept;
		int getSunCount() noexcept;

		constexpr const int getMaxSunCount() noexcept { return 99990; }

		void setSunCount(int value) noexcept;
	private:
		struct LevelMgrMem
		{
			int levelMgrSeedAddr = 0;
			int levelMgrSunAddr = 0;
			int levelMgrPlantAddr = 0;
			int levelMgrZombieAddr = 0;
			int levelMgrProjAddr = 0;
			int levelMgrCollAddr = 0;
		};
		LevelMgrMem m_lmMemory;
	private:
		int m_interface = 0;
		int m_gameModule = 0;
	};
	inline std::unique_ptr<levelManager> iLevelManager;
}