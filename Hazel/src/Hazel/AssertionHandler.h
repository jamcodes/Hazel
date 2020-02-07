#pragma once

#include "Assertion.h"
#include "Log.h"

namespace Hazel
{
struct LoggingHandler {
    template <typename Logger, typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                       Args&&... args)
    {
        logger->error("Assertion failed [{0}:{1}]: '{2}' -> ", loc.file, loc.line, expr);
        logger->error(std::forward<Args>(args)...);
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
