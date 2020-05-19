#pragma once

#include <stdexcept>

#include "Hazel/Core/Assertion.h"
#include "Hazel/Core/Log.h"

namespace Hazel {
struct LoggingHandler {
    template <typename Logger, typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                       Args&&... args) noexcept
    {
        logger->error("Assertion failed [{0}:{1}]: '{2}' -> ", loc.file, loc.line, expr);
        logger->error(std::forward<Args>(args)...);
    }
};

struct DebugTrapHandler {
    template <typename Logger, typename... Args>
    [[noreturn]] static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                                    Args&&... args) noexcept
    {
        LoggingHandler::handle(loc, expr, logger, std::forward<Args>(args)...);
        CONTRACT_DEBUG_TRAP();
    }
};

struct AbortingHandler {
    template <typename Logger, typename... Args>
    [[noreturn]] static void handle(Hazel::source_location const& loc, const char* const expr, Logger& logger,
                                    Args&&... args) noexcept
    {
        LoggingHandler::handle(loc, expr, logger, std::forward<Args>(args)...);
        std::abort();
    }
};

struct BrokenContractError : std::logic_error {
    using std::logic_error::logic_error;
    using std::logic_error::what;
};

struct ThrowingContractHandler {
    template <typename Logger, typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Args&&... args)
    {
        std::ostringstream oss{"[broken contract]: "};
        oss << loc.file << ":" << loc.line << " -- \"" << expr << "\" -- ";
        (..., [&]() { oss << args << ", "; });
        throw BrokenContractError{std::move(oss).str()};
    }
};

template <typename Handler>
struct CoreContractHandler {
    template <typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Args&&... args) noexcept
    {
        Handler::handle(loc, expr, ::Hazel::Log::GetCoreLogger(), std::forward<Args>(args)...);
    }
};

template <typename Handler>
struct HAZEL_API ClientContractHandler {
    template <typename... Args>
    static void handle(Hazel::source_location const& loc, const char* const expr, Args&&... args)
    {
        Handler::handle(loc, expr, ::Hazel::Log::GetClientLogger(), std::forward<Args>(args)...);
    }
};

using CoreLoggingHandler = CoreContractHandler<LoggingHandler>;
using CoreDebugTraphandler = CoreContractHandler<DebugTrapHandler>;
using CoreAbortingHandler = CoreContractHandler<AbortingHandler>;

using ClientLoggingHandler = ClientContractHandler<LoggingHandler>;
using ClientDebugTraphandler = ClientContractHandler<DebugTrapHandler>;
using ClientAbortingHandler = ClientContractHandler<AbortingHandler>;

struct DefaultCoreHandler :
#ifdef HZ_CONTRACT_USE_DEBUGTRAP_HANDLER  // use by default while in development
    CoreDebugTraphandler
#elif HZ_CONTRACT_USE_ABORTING_HANDLER  // use in tests and maybe in release builds with debug info?
    CoreAbortingHandler
#elif HZ_CONTRACT_USE_THROWING_HANDLER  // use in tests and maybe in release builds with debug info?
    ThrowingContractHandler
#else                                   // default to informing - release builds with debug info
    Hazel::CoreLoggingHandler
#endif
    ,
    Hazel::ContractLevel<::Hazel::global_contract_level()> {
};
struct HAZEL_API DefaultClientHandler :
#ifdef HZ_CONTRACT_USE_DEBUGTRAP_HANDLER  // use by default while in development
    ClientDebugTraphandler
#elif HZ_CONTRACT_USE_ABORTING_HANDLER  // use in tests and maybe in release builds with debug info?
    ClientAbortingHandler
#elif HZ_CONTRACT_USE_THROWING_HANDLER  // use in tests and maybe in release builds with debug info?
    ThrowingContractHandler
#else                                   // default to informing - release builds with debug info
    Hazel::ClientLoggingHandler
#endif
    ,
    Hazel::ContractLevel<::Hazel::global_contract_level()> {
};

}  // namespace Hazel
