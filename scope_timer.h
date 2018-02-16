// See LICENSE for license details.

#ifndef SCOPE_TIMER_H_INCLUDED
#define SCOPE_TIMER_H_INCLUDED

#include <chrono>
#include <utility>

// This code makes use of Andrei Alexandrescu's scope guard concept
// and his trick for their syntactic abstraction with lambdas.
//
// Usage:
// TIME_SCOPE_*(time_var) { BODY(time_var); }
namespace __scope_timer_detail {

/** Class measuring the exection time from construction to destruction.
 * At destruction, calls a function with the elapsed time as parameter.
 * The time parameter of "f" is of type Unit::rep, i.e.
 * int for std::chrono::(microseconds|milliseconds|seconds|...).
 */
template <typename Func, typename Unit = std::chrono::milliseconds>
struct ScopeTimer {
    explicit ScopeTimer(Func&& f):
        f(std::forward<Func>(f)),
        start(std::chrono::steady_clock::now())
    {}

    ~ScopeTimer() {
        auto end = std::chrono::steady_clock::now();
        f(std::chrono::duration_cast<Unit>(end - start).count());
    }

private:
    Func f;
    std::chrono::steady_clock::time_point start;
};

// The purpose of this template argument is only that the following
// operator<<'s template argument "Unit" can be deducted by the
// compiler.
template <typename T>
struct ScopeTimerRegister {};

template <typename Func, typename Unit>
inline ScopeTimer<Func, Unit> operator<<(ScopeTimerRegister<Unit>, Func&& f)
{
    return ScopeTimer<Func, Unit>(std::forward<Func>(f));
}

}

#define __SCOPE_TIMER_PP_CONCAT_IMPL(s1, s2) s1##s2
#define __SCOPE_TIMER_PP_CONCAT(s1, s2) __SCOPE_TIMER_PP_CONCAT_IMPL(s1, s2)

#ifdef __COUNTER__
#define __SCOPE_TIMER_PP_ANONYMOUS_VARIABLE(str) \
    __SCOPE_TIMER_PP_CONCAT(str, __COUNTER__)
#else
#define __SCOPE_TIMER_PP_ANONYMOUS_VARIABLE(str) \
    __SCOPE_TIMER_PP_CONCAT(str, __LINE__)
#endif

#define TIME_SCOPE_UNIT(unit, var) \
    auto __SCOPE_TIMER_PP_ANONYMOUS_VARIABLE(__scope_timer) \
        = __scope_timer_detail::ScopeTimerRegister<unit>() << [&](unit::rep var)

// Integer valued timings
#define TIME_SCOPE_iMICRO(var) \
    TIME_SCOPE_UNIT(std::chrono::microseconds, var)
#define TIME_SCOPE_iMILLI(var) \
    TIME_SCOPE_UNIT(std::chrono::milliseconds, var)
#define TIME_SCOPE_iSEC(var) \
    TIME_SCOPE_UNIT(std::chrono::seconds, var)


namespace __frac_units {
using microseconds = std::chrono::duration<double, std::micro>;
using milliseconds = std::chrono::duration<double, std::milli>;
using seconds = std::chrono::duration<double>;
}

// Timings with fractional parts
#define TIME_SCOPE_MICRO(var) \
    TIME_SCOPE_UNIT(__frac_units::microseconds, var)
#define TIME_SCOPE_MILLI(var) \
    TIME_SCOPE_UNIT(__frac_units::milliseconds, var)
#define TIME_SCOPE_SEC(var) \
    TIME_SCOPE_UNIT(__frac_units::seconds, var)

#endif
