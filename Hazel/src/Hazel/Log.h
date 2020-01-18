#pragma once

#include "Core.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace Hazel {

class HAZEL_API Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() noexcept { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() noexcept
    {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

struct HAZEL_API LoggingHandler {
    template <typename Logger, typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                       Args&&... args)
    {
        logger->error("Assertion failed [{0}:{1}]: '{2}' -> {3}", loc.file, loc.line, expr,
                      std::forward<Args>(args)...);
    }
};

struct HAZEL_API CoreLoggingHandler {
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

}  // namespace Hazel

// Core log macros
#define HZ_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...) ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define HZ_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...) ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)