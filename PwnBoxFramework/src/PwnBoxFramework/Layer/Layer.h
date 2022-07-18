#pragma once

#include "PwnBoxFramework/Base/Base.h"
#include <string>

namespace PwnBoxFramework
{
#if PWNBOX_INTERNAL_HACK
	class Layer
	{
	public:
		Layer(const std::string& name = "Debug Layer");
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
#else
	class Layer
	{
	public:
		Layer(const std::string& name = "Debug Layer");
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
#endif
}