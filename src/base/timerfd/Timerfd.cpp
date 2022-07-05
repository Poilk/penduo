//
// Created by poilk on 2022/7/4.
//

#include "Timerfd.h"

namespace penduo {

struct timespec Timerfd::how_much_time_from_now(Timestamp when) {
  auto now = Timestamp::now();
  auto microseconds = Timestamp::differ<Timestamp::TsMicroseconds>(now, when);
  microseconds = std::max(microseconds, static_cast<decltype(microseconds)>(100));
  struct timespec ts{};
  ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::MicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>((microseconds % Timestamp::MicroSecondsPerSecond) * 1000);
  return ts;
}

} // penduo
