#include "Player.h"
#include <vector>
#include <Windows.h>

namespace PwnBoxFramework
{

	void player::initPlayer() noexcept
	{
		m_pMemory.BasePlayerAddr = memory->GetPointerAddress(m_gameModule, { 0x94C });
		m_pMemory.PlayerNameAddr = memory->GetPointerAddress(m_pMemory.BasePlayerAddr, { 0x04 });
		m_pMemory.fertCountAddr = memory->GetPointerAddress(m_pMemory.BasePlayerAddr, { 0x220 });
		m_pMemory.chCountAddr = memory->GetPointerAddress(m_pMemory.BasePlayerAddr, { 0x250 });
		m_pMemory.bgsprayCountAddr = memory->GetPointerAddress(m_pMemory.BasePlayerAddr, { 0x224 });
		m_pMemory.CoinCountAddr = memory->GetPointerAddress(m_pMemory.BasePlayerAddr, { 0x50 });
	}

	player::player(const int& gameModule) noexcept
		: m_gameModule(gameModule) { initPlayer(); }

	std::string player::getPlayerName() noexcept { return memory->ReadProcMemory<std::string>(m_pMemory.PlayerNameAddr); }
	int player::getCoinCount() noexcept { return memory->ReadProcMemory<int>(m_pMemory.CoinCountAddr); }
	int player::getFertilizerCount() noexcept { return memory->ReadProcMemory<int>(m_pMemory.fertCountAddr); }
	int player::getBugSprayCount() noexcept { return memory->ReadProcMemory<int>(m_pMemory.bgsprayCountAddr); }
	int player::getChocolateCount() noexcept { return memory->ReadProcMemory<int>(m_pMemory.chCountAddr); }
	int player::getGameMode() noexcept { return memory->ReadProcMemory<int>(m_gameModule, { 0x918 }); }

	void player::setFertilizerCount(int value) noexcept { memory->WriteProcMemory<int>(m_pMemory.fertCountAddr, value); }
	void player::setBugSprayCount(int value) noexcept { memory->WriteProcMemory<int>(m_pMemory.bgsprayCountAddr, value); }
	void player::setChocolateCount(int value) noexcept { memory->WriteProcMemory<int>(m_pMemory.chCountAddr, value); }
	void player::setCoinCount(int value) noexcept { memory->WriteProcMemory<int>(m_pMemory.CoinCountAddr, value); }

}