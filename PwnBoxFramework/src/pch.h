#pragma once

#include "PwnBoxFramework/Base/Base.h"

#include <functional>
#include <memory.h>
#include <ostream>
#include <istream>
#include <sstream>
#include <string>
#include <random>
#include <ctime>
#pragma warning(disable: C4005)
#if defined(PWNBOX_PLATFORM_WIN)
	#include <Windows.h>
	#include <TlHelp32.h>
#endif