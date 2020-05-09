#pragma once

#include "Hazel/Core/Assertion.h"
#include "Hazel/Core/Log.h"

#if defined(_MSC_VER)
#define DEBUG_TRAP() __debugbreak()
#elif defined(__has_builtin)
#if __has_builtin(__builtin_debugtrap)
#define DEBUG_TRAP() __builtin_debugtrap()
#else
#define DEBUG_TRAP()
#endif
#elif defined(__GNUC__)
#include <signal.h>
#if defined(SIGTRAP)
#define DEBUG_TRAP() raise(SIGTRAP)
#else
#define DEBUG_TRAP()
#endif
#else
#define DEBUG_TRAP()
#endif

namespace Hazel
{
struct LoggingHandler {
    template <typename Logger, typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                       Args&&... args)
    {
        logger->error("Assertion failed [{0}:{1}]: '{2}' -> ", loc.file, loc.line, expr);
        logger->error(std::forward<Args>(args)...);
        DEBUG_TRAP();
    }
};

struct CoreLoggingHandler {
    template <typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Args&&... args)
    {
        ::Hazel::LoggingHandler::handle(loc, expr, ::Hazel::Log::GetCoreLogger(),
                                        std::forward<Args>(args)...);
    }
};

struct HAZEL_API ClientLoggingHandler {
    template <typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Args&&... args)
    {
        ::Hazel::LoggingHandler::handle(loc, expr, ::Hazel::Log::GetClientLogger(),
                                        std::forward<Args>(args)...);
    }
};

struct DefaultCoreHandler : Hazel::CoreLoggingHandler, Hazel::Enforce { };
struct HAZEL_API DefaultClientHandler : Hazel::ClientLoggingHandler, Hazel::Enforce { };

} // namespace Hazel
