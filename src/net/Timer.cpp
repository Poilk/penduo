//
// Created by poilk on 2022/6/29.
//

#include "Timer.h"

namespace penduo {

Timer::Timer(TimerCallback cb, Timestamp when, int64_t interval_ms) :
    callback_(std::move(cb)),
    expiration_ts_(when),
    repeat_(interval_ms > 0),
    interval_ms_(interval_ms){
}

} // penduo