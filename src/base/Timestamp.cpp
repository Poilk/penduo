//
// Created by poilk on 2022/6/28.
//

#include "Timestamp.h"

namespace penduo {

Timestamp::Timestamp() :
    time_point_(TsDuration::zero()) {
}

Timestamp::Timestamp(TsTimePoint ts_time_point) :
    time_point_(ts_time_point) {
}

bool Timestamp::operator<(const Timestamp &rhs) const {
  return time_point_ < rhs.time_point_;
}

Timestamp Timestamp::now() {
  return Timestamp(TsClock::now());
}

Timestamp Timestamp::invalid() {
  return Timestamp{};
}

} // penduo