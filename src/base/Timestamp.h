//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_BASE_TIMESTAMP_H_
#define PENDUO_SRC_BASE_TIMESTAMP_H_

#include <chrono>

#include "base/copyable.h"

namespace penduo {

class Timestamp : copyable {
 public:
  typedef std::chrono::steady_clock TsClock;
  typedef TsClock::duration TsDuration;
  typedef TsClock::time_point TsTimePoint;

  typedef std::chrono::seconds TsS;
  typedef std::chrono::milliseconds TsMS;

  Timestamp();
  explicit Timestamp(TsTimePoint ts_time_point);
  bool operator<(const Timestamp &rhs) const;

  template<class T>
  static int64_t differ(Timestamp st, Timestamp ed);

  template<class T>
  static Timestamp add(Timestamp timestamp, int64_t count);

  bool valid() const { return time_point_.time_since_epoch() != TsDuration::zero(); }

  static Timestamp now();
  static Timestamp invalid();
 private:

  TsTimePoint time_point_;
};

template<class T>
Timestamp Timestamp::add(Timestamp timestamp, int64_t count) {
  //todo use static_assert to check T
  Timestamp::TsTimePoint new_ts = timestamp.time_point_ + T{count};
  return Timestamp{new_ts};
}

template<class T>
int64_t Timestamp::differ(Timestamp st, Timestamp ed) {
  //todo use static_assert to check T
  return std::chrono::duration_cast<T>(ed.time_point_ - st.time_point_).count();
}

} // penduo

#endif //PENDUO_SRC_BASE_TIMESTAMP_H_
