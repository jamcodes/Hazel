#pragma once

#include <chrono>

namespace Hazel {
class Timestep {
public:
    using clock_type = std::chrono::system_clock;
    using duration_type = typename clock_type::duration;
    using time_point_type = typename clock_type::time_point;

    inline duration_type tick() noexcept
    {
        auto const old_time{time_};
        time_ = clock_type::now();
        return time_ - old_time;
    }

    static constexpr inline auto asFloat(duration_type duration) noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
    }

private:
    time_point_type time_{clock_type::now()};
};
}  // namespace Hazel
