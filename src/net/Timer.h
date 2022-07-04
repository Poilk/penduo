//
// Created by poilk on 2022/6/29.
//

#ifndef PENDUO_SRC_NET_TIMER_H_
#define PENDUO_SRC_NET_TIMER_H_

#include <memory>
#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "net/Callbacks.h"

namespace penduo {

class Timer : noncopyable {
 public:
  typedef std::shared_ptr<Timer> SharedPtr;
  typedef std::weak_ptr<Timer> WeakPtr;

  Timer(TimerCallback cb, Timestamp when, int64_t interval_ms = 0);
  ~Timer() = default;

  Timestamp get_expiration_ts()const {return expiration_ts_;}
  bool is_repeat()const {return repeat_;}

  //void restart(Timestamp now);

  void run(){
    callback_();
  }

 private:
  TimerCallback callback_;
  Timestamp expiration_ts_;
  const int repeat_;
  const int64_t interval_ms_;
};

} // penduo

#endif //PENDUO_SRC_NET_TIMER_H_
