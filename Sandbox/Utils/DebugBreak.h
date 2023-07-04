#pragma once

#include "PlatformDetection.h"

//Taken from  TheCherno's Hazel engine. 

#ifdef _DEBUG

#if defined(PLATFORM_WINDOWS)
#define SB_DEBUGBREAK() __debugbreak()
#elif defined(PLATFORM_LINUX)
#include <signal.h>
#define SB_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define SB_ENABLE_ASSERTS

#else
#define SB_DEBUGBREAK()
#endif