#pragma once

#include <PwnBoxFramework.h>

namespace PwnBoxFramework
{
	class plants
	{
	private:
		void init() noexcept;
	public:
		plants() noexcept = default;
		plants(const int& gameModule) noexcept;
		int getPlantHealth() noexcept;		// { return m_gameModule + 0x140680; }
		int getPlantMaxHP() noexcept;
		int getPlantId() noexcept;
		int getCurrentPlantCount() noexcept;
		int getPlantRechargeTime() noexcept; // { return m_gameModule + 0x91E55; }
		float getPlantX(int index) noexcept;
		float getPlantY(int index) noexcept;
		void OnUpdate() noexcept;
	private:
		struct PlantMemory
		{
			int plantBaseAddr;
			int plantIdAddr;
			int plantHealthAddr;
			int plantMaxHPAddr;
			int plantCurrentCount;
			int plantRechargeAddr;
			int plantposXAddr;
			int plantposYAddr;
		};
		PlantMemory m_pMemory;
	private:
		int m_interface = 0;
		int m_gameModule = 0;
	};
}