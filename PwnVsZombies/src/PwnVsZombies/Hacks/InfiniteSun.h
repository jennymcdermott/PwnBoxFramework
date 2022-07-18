#pragma once


namespace PwnBoxFramework
{
	class InfiniteSun
	{
	public:
		void OnUpdate() noexcept;
		bool isRunning() const noexcept;
		void disableHack() noexcept;
	private:
		void enableHack() noexcept;
	private:
		bool m_iRunning = false;
		int m_prevSunValue = 0;
	};
}