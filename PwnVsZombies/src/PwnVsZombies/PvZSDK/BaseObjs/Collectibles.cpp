#include "Collectibles.h"
#include <vector>
#include <Windows.h>

namespace PwnBoxFramework
{
	void collectibles::init() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x868 });
		m_cMemory.collectiblesBaseAddr = memory->GetPointerAddress(m_interface, { 0x0 });
	}

	collectibles::collectibles(int gameModule) noexcept
		: m_gameModule(gameModule) { init(); }

	int collectibles::getTotalCollectibles() noexcept
	{
		return 0;
	}

	int collectibles::getMaxCollectibles() noexcept
	{
		return 0;
	}

	int collectibles::getCurrentCount() noexcept
	{
		return 0;
	}

	int collectibles::getspawnTimer() noexcept
	{
		return 0;
	}

	int collectibles::getdespawnTimer() noexcept
	{
		return 0;
	}

	float collectibles::getX() noexcept
	{
		return 0.0f;
	}

	float collectibles::getY() noexcept
	{
		return 0.0f;
	}

	bool collectibles::isClicked() noexcept
	{
		return false;
	}
}