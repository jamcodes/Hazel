#pragma once

#include "asserts.h"

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
