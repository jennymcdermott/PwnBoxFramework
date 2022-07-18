#include "Projectiles.h"

namespace PwnBoxFramework
{
	void projectile::init() noexcept
	{
	}

	projectile::projectile(int gameModule) noexcept
		: m_gameModule(gameModule)
	{
		init();
	}

	void projectile::OnUpdate() noexcept
	{
	}
}