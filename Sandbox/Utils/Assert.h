#pragma once

#include "Logger.h"
#include "DebugBreak.h"

//Taken from TheCherno's Hazel assert.h
#include <filesystem>

#define SB_EXPAND_MACRO(x) x
#define SB_STRINGIFY_MACRO(x) #x

#ifdef SB_ENABLE_ASSERTS

// Alternatively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define SB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { SB##type##ERROR(msg, __VA_ARGS__); SB_DEBUGBREAK(); } }
#define SB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define SB_INTERNAL_ASSERT_NO_MSG(type, check) SB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", SB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define SB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define SB_INTERNAL_ASSERT_GET_MACRO(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SB_INTERNAL_ASSERT_WITH_MSG, SB_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define SB_ASSERT(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define SB_CORE_ASSERT(...) SB_EXPAND_MACRO( SB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define SB_ASSERT(...)
#define SB_CORE_ASSERT(...)
#endif
