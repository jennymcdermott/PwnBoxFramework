#include "AppLayer.h"
#include "PwnVsZombies/PvZSDK/Engine.h"

namespace PwnBoxFramework
{
	void AppLayer::OnAttach()
	{
		memory->HookProcess("popcapgame1.exe");
		engine = std::make_unique<Engine>();
		iLevelManager = std::make_unique<levelManager>();
		engine->init();
		iLevelManager->init(engine->getGameAddr());
	}

	void AppLayer::OnDetach()
	{
		m_infiniteCoin.disableHack();
		m_infiniteSun.disableHack();
	}
	void AppLayer::OnUpdate()
	{
		m_infiniteCoin.OnUpdate();
		m_infiniteSun.OnUpdate();
	}
}