#pragma once

#include "CInterface/CInteface.h"
#include "Level/LevelManager.h"
#include "BaseObjs/Plants.h"
#include "BaseObjs/Zombies.h"
#include "Player.h"

namespace PwnBoxFramework
{
	class Engine
	{
	public:
		Engine() noexcept;
		void init() noexcept;
		CInterface Interface;
		zombies Zombies;
		plants Plants;
		player Player;
		int getGameAddr() noexcept;
		void OnUpdate() noexcept;
	private:
		int m_gameModule;
		int m_baseGameAddr;
	};
	inline std::unique_ptr<Engine> engine;
}