//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_NET_TIMERQUEUE_H_
#define PENDUO_SRC_NET_TIMERQUEUE_H_

#include <set>
#include <memory>

#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "net/TimerHandle.h"
#include "net/Callbacks.h"
#include "net/Channel.h"
#include "net/Timer.h"

namespace penduo {

class EventLoop;

class TimerQueue : noncopyable{
 public:
  explicit TimerQueue(EventLoop *loop);
  ~TimerQueue();

  // Schedules the callback to be run at given time,
  // repeats if interval > 0.0
  // Must be thread safe. Usually be called from another threads.
  //todo support cancel
  TimerHandle add_timer(TimerCallback cb, Timestamp when, int64_t interval_ms = 0);
  //void cancel(TimerHandle);

 private:
  typedef std::set<std::pair<Timestamp, Timer::SharedPtr>> TimerList;

  std::vector<Timer::SharedPtr> get_expired(Timestamp now);
  void add_timer_in_loop(Timer::SharedPtr timer);
  bool insert(Timer::SharedPtr timer);
  void handle_read();
  void reset();

  EventLoop *loop_;
  const int timerfd_;
  Channel timerfd_channel_;
  TimerList timer_list_;
};

} // penduo

#endif //PENDUO_SRC_NET_TIMERQUEUE_H_
