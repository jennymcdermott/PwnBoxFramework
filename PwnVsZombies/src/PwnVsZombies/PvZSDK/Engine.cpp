#include "Engine.h"
#include <PwnBoxFramework.h>
#include <iostream>

namespace PwnBoxFramework
{
	Engine::Engine() noexcept 
	{

	}

	void Engine::init() noexcept
	{
		m_gameModule = memory->GetModuleAddress("popcapgame1.exe");
		m_baseGameAddr = m_gameModule + 0x329670;
		Interface = CInterface(m_baseGameAddr);
		Player = player(m_baseGameAddr);
		Plants = plants(m_baseGameAddr);
		Zombies = zombies(m_baseGameAddr);
	}

	int Engine::getGameAddr() noexcept { return m_baseGameAddr; }

	void Engine::OnUpdate() noexcept
	{
		/*
		* All addresses will need to be updated no matter what
		* put all objects that need to be updated here so that
		* they can be updated without having to do some funky shit
		*/
		Plants.OnUpdate();
		Zombies.OnUpdate();
	}
}