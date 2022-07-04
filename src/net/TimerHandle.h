//
// Created by poilk on 2022/6/29.
//

#ifndef PENDUO_SRC_NET_TIMERHANDLE_H_
#define PENDUO_SRC_NET_TIMERHANDLE_H_

#include <memory>
#include <utility>

#include "base/copyable.h"

namespace penduo {

class Timer;
class TimerQueue;

class TimerHandle : copyable{
 public:
  explicit TimerHandle(std::weak_ptr<Timer> weak_ptr) : weak_timer_(std::move(weak_ptr)){
  }
  TimerHandle() = delete;

  friend class TimerQueue;

 private:
  std::weak_ptr<Timer> weak_timer_;
};

} // penduo

#endif //PENDUO_SRC_NET_TIMERHANDLE_H_
