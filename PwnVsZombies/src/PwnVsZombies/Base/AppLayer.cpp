#include "AppLayer.h"
#include "PwnVsZombies/PvZSDK/Engine.h"

namespace PwnBoxFramework
{
	void AppLayer::OnAttach()
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONIN$", "r", stdin);
		freopen_s(&file, "CONOUT$", "w", stdout);
		freopen_s(&file, "CONOUT$", "w", stderr);

		engine = std::make_unique<Engine>();
		iLevelManager = std::make_unique<levelManager>();

		memory->HookProcess("popcapgame1.exe");
		engine->init();
		iLevelManager->init(engine->getGameAddr());
		m_oldCoinCount = engine->Player.getCoinCount();
		if (!m_isPrinted)
		{
			std::cout << "PwnVsZombies: The best Plants vs Zombies trainer!\n";
			std::cout << "-------------------------------------------------\n";
			std::cout << "[NUMPAD2]: Infinite Sun hack!\n";
			std::cout << "[NUMPAD5]: Infinite Money Hack!\n";
			std::cout << "-------------------------------------------------\n";
			std::cout << "Player Name: " << engine->Player.getPlayerName() << "\n";
			std::cout << "Game Mode: " << engine->Player.getGameMode() << "\n";
			std::cout << "Old Coin Count: " << std::dec << engine->Player.getCoinCount() << "\n";
			m_isPrinted = !m_isPrinted;
		}
	}

	void AppLayer::OnDetach()
	{
		FreeConsole();
		m_infiniteCoin.disableHack();
		m_infiniteSun.disableHack();
	}
	void AppLayer::OnUpdate()
	{
		m_infiniteCoin.OnUpdate();
		m_infiniteSun.OnUpdate();
	}
}