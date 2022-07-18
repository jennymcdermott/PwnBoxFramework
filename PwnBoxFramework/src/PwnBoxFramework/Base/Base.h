#pragma once

#if defined(_WIN32)
	#define PWNBOX_PLATFORM_WIN
	
	#define PWNBOX_DLL_INJECTOR 0
	#ifdef PWNBOX_BUILD_DLL // If you're planning on making a dll hack with this lib, define this!
		#undef PWNBOX_DLL_INJECTOR
		#undef PWNBOX_EXTERNAL_HACK
		#define PWNBOX_INTERNAL_HACK 1
		#define PWNBOX_EXTERNAL_HACK 0
		#define PWNBOX_DLL_INJECTOR 0
	#elif PWNBOX_DLL_INJECTOR
		#define PWNBOX_INTERNAL_HACK 0
		#define PWNBOX_EXTERNAL_HACK 0
	#else 
		#define PWNBOX_INTERNAL_HACK 0
		#define PWNBOX_EXTERNAL_HACK 1
	#endif
#endif

#include <memory>

namespace PwnBoxFramework
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}