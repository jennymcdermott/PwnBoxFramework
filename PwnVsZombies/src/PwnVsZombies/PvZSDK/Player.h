#pragma once

#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class player
	{
	private:
		void initPlayer() noexcept;
	public:
		player() noexcept = default;
		player(const int& gameModule) noexcept;

		int getBaseGame() noexcept { return m_gameModule; }
		int getPlayer() noexcept { return m_pMemory.BasePlayerAddr; }

		int plantUpgrade() noexcept { return m_gameModule + 0x935F1; }
		int plantSurface() noexcept { return m_gameModule + 0x127EF; }

		std::string getPlayerName() noexcept;
		int getGameMode() noexcept;

		int getCoinCount() noexcept;
		int getFertilizerCount() noexcept;
		int getBugSprayCount() noexcept;
		int getChocolateCount() noexcept;
		constexpr const int getMaxFertilizerCount() noexcept { return 9999; }
		constexpr const int getMaxBugSprayCount() noexcept { return 9999; }
		constexpr const int getMaxChocolateCount() noexcept { return 9999; }
		constexpr const int getMaxCoinCount() noexcept { return 99999; }

		void setFertilizerCount(int value) noexcept;
		void setBugSprayCount(int value) noexcept;
		void setChocolateCount(int value) noexcept;
		void setCoinCount(int value) noexcept;

	private:
		struct PlayerMemory
		{
			int BasePlayerAddr;
			int PlayerNameAddr;
			int CoinCountAddr;
			int bgsprayCountAddr;
			int fertCountAddr;
			int chCountAddr;
		};
		PlayerMemory m_pMemory;
	private:
		int m_gameModule = 0;
	};
}