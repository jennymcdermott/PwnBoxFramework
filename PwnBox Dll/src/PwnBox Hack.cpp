#include <PwnBoxFramework.h>
#include <PwnBoxFramework/Base/Entrypoint.h>
#include "PwnBox Hack/Base/AppLayer.h"

namespace PwnBoxFramework
{
	class PwnBoxHack : public Application
	{
	public:
		PwnBoxHack() noexcept
			: Application() { PushLayer(new AppLayer); }
		virtual ~PwnBoxHack() = default;
	};

	Application* CreateApp() noexcept { return new PwnBoxHack(); }
}