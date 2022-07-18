#include "InfiniteMoney.h"
#include <PwnBoxFramework.h>
#include "PwnVsZombies/PvZSDK/Engine.h"

namespace PwnBoxFramework
{
	void InfiniteCoin::OnUpdate() noexcept { engine->Player.setCoinCount(engine->Player.getMaxCoinCount()); }

	void InfiniteCoin::disableHack() noexcept
	{
		if (m_prevCoinValue != engine->Player.getCoinCount() || m_prevCoinValue != 0)
			engine->Player.setCoinCount(m_prevCoinValue);
	}

	bool InfiniteCoin::isEnabled() noexcept { return m_iRunning; }

	void InfiniteCoin::enableHack() noexcept
	{
		if (!m_iRunning && m_prevCoinValue < engine->Player.getMaxCoinCount())
			m_prevCoinValue = engine->Player.getCoinCount();


		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			m_iRunning = !m_iRunning;

			if (!m_iRunning)
				disableHack();
		}

		if (m_iRunning)
			enableHack();
	}
}