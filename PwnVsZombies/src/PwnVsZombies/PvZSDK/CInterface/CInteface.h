#pragma once
namespace PwnBoxFramework
{

	class CInterface
	{
	private:
		void init() noexcept;
	public:
		CInterface() noexcept = default;
		CInterface(const int& gameModule) noexcept;

		int getInterface() noexcept;
		bool isInLevel() noexcept;
		bool isPaused() noexcept;
	private:
		struct InterfaceData
		{
			int gameInterface = 0;
			int gameState = 0;
			int isPaused = 0;
		};
		InterfaceData m_iMemory;
	private:
		int m_gameModule;
	};
}