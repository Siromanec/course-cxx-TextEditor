//
// Created by ADMIN on 25-Feb-24.
//

#ifndef COROUTINES_GAMECLOCK_HPP
#define COROUTINES_GAMECLOCK_HPP
#include <cstdint>
#include <chrono>

typedef uint64_t tick_t;

class GameClock {
  tick_t ticks;
public:
  [[nodiscard]] tick_t getTicks() const {
    return ticks;
  }

private:
  std::chrono::nanoseconds remainder;
  const std::chrono::milliseconds msPerTick;

public:

  explicit GameClock(std::chrono::milliseconds msPerSecond): ticks{}, remainder{}, msPerTick{msPerSecond} {}
  explicit GameClock(size_t msPerTick): ticks{}, remainder{}, msPerTick{msPerTick} {}

  GameClock& operator+=(std::chrono::nanoseconds delta_time) {
    remainder += delta_time;
    return *this;
  }

  bool hasNext() {
    return remainder > std::chrono::duration_cast<std::chrono::nanoseconds>(msPerTick);
  }
  tick_t getNext() {
    remainder -= std::chrono::duration_cast<std::chrono::nanoseconds>(msPerTick);
    return ++ticks;
  }

  GameClock(const GameClock&) = delete;
  GameClock operator=(const GameClock&) = delete;
  GameClock operator=(GameClock) = delete;
};

//class CustomSteadyClock {
//public:
//  using rep = long long;
//  using period = std::ratio<1>;
//  using duration = std::chrono::duration<rep, period>;
//  using time_point = std::chrono::time_point<CustomSteadyClock>;
//
//  static constexpr bool is_steady = true;
//
//  static time_point now() noexcept {
//    // Implement your custom logic to obtain the current time point
//    // For simplicity, let's just use the system clock as an example
//    return std::chrono::time_point_cast<duration>(std::chrono::system_clock::now());
//  }
//};

class CustomSteadyClock {
public:
  using rep = long long;
  using period = std::ratio<1, 30>; // 30 ticks per second
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<CustomSteadyClock>;

  static constexpr bool is_steady = true;

  static time_point now() noexcept {
    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceEpoch = currentTime.time_since_epoch();
    auto ticks = std::chrono::duration_cast<duration>(timeSinceEpoch).count();
    return time_point(duration(ticks));
  }
};
#endif //COROUTINES_GAMECLOCK_HPP
