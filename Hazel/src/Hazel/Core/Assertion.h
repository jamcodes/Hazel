#pragma once

#include <cstdio>
#include <cstdlib>

#include <utility>

#define CONTRACT_STRINGIFY_IMPL(x) #x
#define CONTRACT_STRINGIFY(x) CONTRACT_STRINGIFY_IMPL(x)

#define CONTRACT_CONCATENATE_IMPL(s1, s2) s1##s2
#define CONTRACT_CONCATENATE(s1, s2) CONTRACT_CONCATENATE_IMPL(s1, s2)

#if defined(__GNUC__) || defined(__clang__)
// Use `!!(cond)` to allow implicit conversions to bool?
// #define CONTRACT_LIKELY(x) __builtin_expect(!!(x), 1)
// #define CONTRACT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define CONTRACT_LIKELY(x) __builtin_expect((x), 1)
#define CONTRACT_UNLIKELY(x) __builtin_expect((x), 0)
#else
// #define CONTRACT_LIKELY(x) (!!(x))
// #define CONTRACT_UNLIKELY(x) (!!(x))
#define CONTRACT_LIKELY(x) (x)
#define CONTRACT_UNLIKELY(x) (x)
#endif

// From Microsoft's GSL
// CONTRACT_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#if defined(_MSC_VER)
#define CONTRACT_ASSUME(cond) __assume(cond)
#define CONTRACT_MARK_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define CONTRACT_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
// #define CONTRACT_ASSUME(cond) if(!CONTRACT_LIKELY(cond)) { __builtin_unreachable(); }
#define CONTRACT_MARK_UNREACHABLE() __builtin_unreachable()
#elif defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
#define CONTRACT_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
// #define CONTRACT_ASSUME(cond) if(!CONTRACT_LIKELY(cond)) { __builtin_unreachable(); }
#define CONTRACT_MARK_UNREACHABLE() __builtin_unreachable()
#endif
#else
#define CONTRACT_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
// #define CONTRACT_ASSUME(cond) if(CONTRACT_LIKELY(cond)) { }
#define CONTRACT_MARK_UNREACHABLE()
#endif

#if !defined(CONTRACT_FORCE_INLINE)
#if defined(__GNUC__)
#define CONTRACT_FORCE_INLINE [[gnu::always_inline]] inline
#elif defined(_MSC_VER)
#define CONTRACT_FORCE_INLINE __forceinline
#elif defined(__has_attribute)
#if __has_attribute(always_inline)
#define CONTRACT_FORCE_INLINE __attribute__((always_inline))
#endif
#else
#define CONTRACT_FORCE_INLINE inline
#endif
#endif

#if defined(_MSC_VER)
#define CONTRACT_DEBUG_TRAP() __debugbreak()
#elif defined(__has_builtin)
#if __has_builtin(__builtin_debugtrap)
#define CONTRACT_DEBUG_TRAP() __builtin_debugtrap()
#else
#define CONTRACT_DEBUG_TRAP()
#endif
#elif defined(__GNUC__)
#include <signal.h>
#if defined(SIGTRAP)
#define CONTRACT_DEBUG_TRAP() raise(SIGTRAP)
#else
#define CONTRACT_DEBUG_TRAP()
#endif
#else
#define CONTRACT_DEBUG_TRAP()
#endif

namespace Hazel {

struct source_location final {
    const char* file;
    unsigned line;
};

#define CONTRACT_SRC_LOC() \
    Hazel::source_location { __FILE__, static_cast<unsigned>(__LINE__) }

namespace detail {
template <bool B, typename True, typename False>
struct IfThenElse {
    using type = False;
};
template <typename True, typename False>
struct IfThenElse<true, True, False> {
    using type = True;
};
}  // namespace detail

template <unsigned Level>
struct ContractLevel {
    using type = ContractLevel;
    using value_type = unsigned;
    constexpr static const unsigned level{Level};
    constexpr operator value_type() const noexcept { return level; }
    constexpr auto operator()() const noexcept { return level; }
};

#define CONTRACT_LEVEL_DEFINE_ASSUME 0
#define CONTRACT_LEVEL_DEFINE_IGNORED 1 // NOTE - IGNORE seems to be defined somewhere to 0
#define CONTRACT_LEVEL_DEFINE_ENFORCE 2
#define CONTRACT_LEVEL_DEFINE_AUDIT 3

using Assume = ContractLevel<CONTRACT_LEVEL_DEFINE_ASSUME>;
using Ignore = ContractLevel<CONTRACT_LEVEL_DEFINE_IGNORED>;
using Enforce = ContractLevel<CONTRACT_LEVEL_DEFINE_ENFORCE>;
using Audit = ContractLevel<CONTRACT_LEVEL_DEFINE_AUDIT>;
// Higher levels can also be used
// using MyExtremelyExpensiveCheck = ContractLevel<4>;
// etc

template <typename Handler, unsigned Level>
struct AssertionLevelIgnoreAssume
    : detail::IfThenElse<Handler::level == Assume::level && (Level == Enforce::level || Level == Assume::level), Assume,
                         Ignore> {
};

template <typename Handler, unsigned Level>
struct AssertionLevelEnforce : detail::IfThenElse<Level <= Handler::level && (Level > Ignore::level), Enforce,
                                                  typename AssertionLevelIgnoreAssume<Handler, Level>::type> {
};

template <typename Handler, typename Level>
using AssertionLevel = typename AssertionLevelEnforce<Handler, Level::level>::type;

struct noop_handler {
    template <typename... Args>
    static void handle(source_location const&, const char* const, Args&&...) noexcept
    {
    }
};

template <class Handler, typename... Args>
constexpr void contract_assert_failed(source_location const& loc, const char* const expr_str,
                                      Args&&... args) noexcept(noexcept(Handler::handle(loc, expr_str,
                                                                                        std::forward<Args>(args)...)))
{
    return Handler::handle(loc, expr_str, std::forward<Args>(args)...), static_cast<void>(0);
}

template <typename Handler, typename Level = Enforce, typename = AssertionLevel<Handler, Level>>
struct ContractAssert final {
    template <typename Expr, typename... Args>
    CONTRACT_FORCE_INLINE static constexpr void do_assert(
        Expr const& expr, source_location const& loc, const char* const expr_str,
        Args&&... args) noexcept(noexcept(Handler::handle(loc, expr_str, std::forward<Args>(args)...)))
    {
        // return CONTRACT_LIKELY(expr())
        //            ? static_cast<void>(0)
        //            //    : contract_assert_failed<Handler>(loc, expr_str, std::forward<Args>(args)...);
        //            // call handler directly? The indirection
        //            : static_cast<void>(Handler::handle(loc, expr_str, std::forward<Args>(args)...));
        if (CONTRACT_LIKELY(expr())) { return static_cast<void>(0); }
        else { return static_cast<void>(Handler::handle(loc, expr_str, std::forward<Args>(args)...)); }
    }
};

template <typename Handler, typename Level>
struct ContractAssert<Handler, Level, Ignore> final {
    template <typename Expr, typename... Args>
    CONTRACT_FORCE_INLINE static constexpr void do_assert(Expr const&, source_location const&, const char* const,
                                                          Args&&...) noexcept
    {
    }
};

template <typename Handler, typename Level>
struct ContractAssert<Handler, Level, Assume> final {
    template <typename Expr, typename... Args>
    CONTRACT_FORCE_INLINE static constexpr void do_assert(Expr const& expr, source_location const&, const char* const,
                                                          Args&&...) noexcept
    {
        return CONTRACT_ASSUME(expr());
        // if (CONTRACT_LIKELY(expr())) { return static_cast<void>(0); }
        // else { CONTRACT_MARK_UNREACHABLE(); }
    }
};

// Cleaner macros due to function template argument deduction, but another layer of indirection - avoid for now
template <typename Expr, typename Handler, unsigned Level, typename... Args>
CONTRACT_FORCE_INLINE constexpr void do_contract_assertion(
    Expr const& expr, source_location const& loc, const char* const expr_str, Handler const, ContractLevel<Level> const,
    Args&&... args) noexcept(noexcept(ContractAssert<Handler, ContractLevel<Level>>::do_assert(expr, loc, expr_str,
                                                                                               std::forward<Args>(
                                                                                                   args)...)))
{
    return ContractAssert<Handler, ContractLevel<Level>>::do_assert(expr, loc, expr_str, std::forward<Args>(args)...);
}

// Cleaner macros due to function template argument deduction, but another layer of indirection - avoid for now
template <typename Expr, typename Handler, typename... Args>
CONTRACT_FORCE_INLINE constexpr void do_contract_assertion(
    Expr const& expr, source_location const& loc, const char* const expr_str, Handler const,
    Args&&... args) noexcept(noexcept(ContractAssert<Handler, Enforce>::do_assert(expr, loc, expr_str,
                                                                                  std::forward<Args>(args)...)))
{
    return ContractAssert<Handler, Enforce>::do_assert(expr, loc, expr_str, std::forward<Args>(args)...);
}

[[noreturn]] inline void assert_failure(const source_location& loc, const char* const expression,
                                        const char* msg = nullptr) noexcept
{
#ifndef CONTRACT_ASSERT_FAILURE_NO_STDIO
    if (msg)
        std::fprintf(stderr, "[assert] %s:%u: Assertion '%s' failed - %s.\n", loc.file, loc.line, expression, msg);
    else
        std::fprintf(stderr, "[assert] %s:%u: Assertion '%s' failed.\n", loc.file, loc.line, expression);
#else
    (void)loc;
    (void)expression;
    (void)msg;
#endif
#ifndef CONTRACT_ASSERT_NO_DEBUG_TRAP
    CONTRACT_DEBUG_TRAP();
#endif
    std::abort();
}

#define HZ_CONTRACT_LEVEL_DEFINE(lEVEL) CONTRACT_CONCATENATE(CONTRACT_LEVEL_DEFINE_, lEVEL)

#ifndef HZ_CONTRACT_LEVEL_CONFIG
#ifdef HZ_CONTRACT_LEVEL
#define HZ_CONTRACT_LEVEL_CONFIG HZ_CONTRACT_LEVEL_DEFINE(HZ_CONTRACT_LEVEL)  // -D HZ_CONTRACT_LEVEL=ASSUME
#else
#define HZ_CONTRACT_LEVEL_CONFIG HZ_CONTRACT_LEVEL_DEFINE(ASSUME)
#endif
#endif

inline constexpr Hazel::ContractLevel<HZ_CONTRACT_LEVEL_CONFIG> global_contract_level{};

}  // namespace Hazel

#if (HZ_CONTRACT_LEVEL_CONFIG >= HZ_CONTRACT_LEVEL_ENFORCE)
// #define CONTRACT_ASSERT(eXPR) \
//     CONTRACT_LIKELY(eXPR) ? static_cast<void>(0) : ::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR);
// #define CONTRACT_ASSERT(eXPR, msg) \
//     CONTRACT_LIKELY(eXPR) ? static_cast<void>(0) : ::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR, msg);
// #define CONTRACT_ASSERT(eXPR) \
//     if(CONTRACT_LIKELY(eXPR)) { } else {::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR); }
// #define CONTRACT_ASSERT(eXPR, msg) \
//     if(CONTRACT_LIKELY(eXPR)) { } else { ::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR, msg); }
#define CONTRACT_ASSERT(eXPR) \
    if(!CONTRACT_LIKELY(eXPR)) {::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR); }
#define CONTRACT_ASSERT(eXPR, msg) \
    if(!CONTRACT_LIKELY(eXPR)) { ::Hazel::assert_failure(CONTRACT_SRC_LOC(), #eXPR, msg); }
#else
#define CONTRACT_ASSERT(eXPR) CONTRACT_ASSUME(eXPR)
#define CONTRACT_ASSERT(eXPR, msg) CONTRACT_ASSUME(eXPR)
#endif

#define HZ_ASSERT(eXPR) CONTRACT_ASSERT(eXPR)
#define HZ_ASSERT(eXPR, msg) CONTRACT_ASSERT(eXPR, msg)

#define HZ_EXPECTS(eXPR, hANDLER, lEVEL, ...)                                                           \
    static_cast<void>(Hazel::ContractAssert<hANDLER, lEVEL>::do_assert([&]() noexcept { return eXPR; }, \
                                                                       CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))

#define HZ_ENSURES(eXPR, hANDLER, lEVEL, ...)                                                           \
    static_cast<void>(Hazel::ContractAssert<hANDLER, lEVEL>::do_assert([&]() noexcept { return eXPR; }, \
                                                                       CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))
