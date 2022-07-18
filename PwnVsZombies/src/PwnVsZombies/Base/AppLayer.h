#pragma once

#include <PwnBoxFramework.h>
#include "PwnVsZombies/Hacks/InfiniteMoney.h"
#include "PwnVsZombies/Hacks/InfiniteSun.h"
namespace PwnBoxFramework
{
	class AppLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
	private:
		InfiniteCoin m_infiniteCoin;
		InfiniteSun m_infiniteSun;
		bool m_isPrinted = false;
		int m_oldCoinCount = 0;
	};
}