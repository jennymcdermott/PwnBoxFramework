#pragma once

namespace PwnBoxFramework
{
	class InfiniteCoin
	{
	public:

		void OnUpdate() noexcept;
		void disableHack() noexcept;
		bool isEnabled() noexcept;
	private:
		void enableHack() noexcept;
	private:
		bool m_iRunning = false;
		int m_prevCoinValue = 0;
	};
}