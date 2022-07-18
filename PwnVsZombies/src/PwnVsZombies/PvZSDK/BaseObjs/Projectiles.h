#pragma once


namespace PwnBoxFramework
{
	class projectile
	{
	private:
		void init() noexcept;
	public:
		projectile() noexcept = default;
		projectile(int gameModule) noexcept;
		void OnUpdate() noexcept;
	private:
		int m_gameModule = 0;
	};
}