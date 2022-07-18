#include <PwnBoxFramework.h>
#include <PwnBoxFramework/Base/Entrypoint.h>
#include "PwnBoxInjector/Base/AppLayer.h"

namespace PwnBoxFramework
{
	class PwnBoxInjector : public Application
	{
	public:
		PwnBoxInjector() : Application({ "PwnBoxFramework: Injector", { 400,400 },false }) { PushLayer(new AppLayer()); }
		virtual ~PwnBoxInjector() = default;
	};

	Application* CreateApp() noexcept {
		return new PwnBoxInjector();
	}
}