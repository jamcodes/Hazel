#pragma once

#include <memory>
#include <utility>

#include "Hazel/Core/Assertion.h"

// Building Hazel as a dynamic library doesn't work at the moment
// not quite sure why - IMGUI symbols are not exported

#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_DYNAMIC_LINK
    #ifdef HZ_BUILD_DLL
        #define HAZEL_API __declspec(dllexport)
    #else
        #define HAZEL_API __declspec(dllimport)
    #endif
#else
    #define HAZEL_API
#endif HZ_DYNAMIC_LINK
#else
    #error Hazel only supports Windows!
#endif

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
