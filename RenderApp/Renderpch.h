# pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <array>
#include <sstream>
#include <string>
#include <optional>
#include <map>
#include <functional>
#include <set>
#include <stdio.h>
#include <cstdint>
#include <limits> 
#include <algorithm>

// Platform Detection

#ifdef _WIN32
	#ifdef _WIN64
	/* Windows x64  */
	#define PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif

#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define PLATFORM_LINUX
#error "Linux is not supported!"
#else
/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

#ifdef DEBUG
	#define EXPAND_MACRO(x) x
	#define STRINGIFY_MACRO(x) #x
#define R_ERROR(msg, ...) std::cout << msg << __VA_ARGS__ << std::endl

	#if defined(PLATFORM_WINDOWS)
		#define DEBUGBREAK() __debugbreak()
	#elif defined(PLATFORM_LINUX)
		#include <signal.h>
		#define HZ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif // PLATFORM_WINDOWS
	
#define INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ##R_ERROR(msg, __VA_ARGS__); DEBUGBREAK(); } }
	#define INTERNAL_ASSERT_WITH_MSG(type, check, ...) INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: ", __VA_ARGS__)
	#define INTERNAL_ASSERT_NO_MSG(type, check) INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro

	#define INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define ASSERT(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#else // else
	#define ASSERT(...)
#endif // DEBUG
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)