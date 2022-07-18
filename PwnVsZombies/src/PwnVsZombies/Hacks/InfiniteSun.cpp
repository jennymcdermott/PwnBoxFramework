#include "InfiniteSun.h"
#include <PwnBoxFramework.h>
#include "PwnVsZombies/PvZSDK/Engine.h"

namespace PwnBoxFramework
{
	void InfiniteSun::OnUpdate() noexcept
	{
		if (!engine->Interface.isInLevel())return;
		if (!m_iRunning && m_prevSunValue < iLevelManager->getMaxSunCount())
			m_prevSunValue = iLevelManager->getSunCount();
		if ((GetAsyncKeyState(VK_NUMPAD2) & 1))
		{
			m_iRunning = !m_iRunning;
			if (!m_iRunning)
				disableHack();
		}

		if (m_iRunning)
			enableHack();
	}

	bool InfiniteSun::isRunning() const noexcept { return m_iRunning; }
	
	void InfiniteSun::disableHack() noexcept
	{
		iLevelManager->setSunCount(iLevelManager->getMaxSunCount());
	}

	void InfiniteSun::enableHack() noexcept
	{
		if (m_prevSunValue == iLevelManager->getSunCount() || m_prevSunValue == 0) return;
		iLevelManager->setSunCount(m_prevSunValue);
	}
}