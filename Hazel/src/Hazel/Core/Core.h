#pragma once

#include <memory>
#include <utility>

#include "Hazel/Core/Assertion.h"

// Building Hazel as a dynamic library doesn't work at the moment
// not quite sure why - IMGUI symbols are not exported

// Platform detection using predefined macros
#ifdef _WIN32
    //Windows x64/x86
    #ifdef _WIN64
        // Windows x64
        #define HZ_PLATFORM_WINDOWS
    #else
        // Windows x86
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    // TARGET_OS_MAC exists on all the platforms
    // so we must check all of them (in this order)
    // to ensure that we're running on MAC
    // and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define HZ_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define HZ_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif
// We also have to check __ANDROID__ before __linux__
// since android is based on the linux kernel
// it has __linux__ defined */
#elif defined(__ANDROID__)
    #define HZ_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define HZ_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
    /* Unknown compiler/platform */
    #error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef HZ_PLATFORM_WINDOWS
    #if HZ_DYNAMIC_LINK
        #ifdef HZ_BUILD_DLL
            #define HAZEL_API __declspec(dllexport)
        #else
            #define HAZEL_API __declspec(dllimport)
        #endif
    #else
        #define HAZEL_API
    #endif
#else
    #error Hazel only supports Windows!
#endif // End of DLL support

#define BIT(x) (1 << (x))

namespace Hazel
{
template<typename T>
using Scope = std::unique_ptr<T>;
// template<typename T>
// using makeScope = std::make_unique<T>;

template<typename T>
using Ref = std::shared_ptr<T>;
// template<typename T>
// using makeRef = std::make_shared<T>;
} // namespace Hazel
