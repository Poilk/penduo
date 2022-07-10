//
// Created by poilk on 2022/6/28.
//

#include <cassert>

#include "base/logging/Logger.h"
#include "net/Timer.h"
#include "net/TimerHandle.h"
#include "net/TimerQueue.h"
#include "net/EventLoop.h"
#include "base/timerfd/TimerfdCreator.h"

namespace penduo {

TimerQueue::TimerQueue(EventLoop *loop) :
    loop_(loop),
    timerfd_(TimerfdCreator::new_default()),
    timerfd_channel_(loop_, timerfd_->get_fd()) {
  timerfd_channel_.set_read_callback(std::bind(&TimerQueue::handle_read, this, std::placeholders::_1));
  timerfd_channel_.enable_reading();
}

TimerQueue::~TimerQueue() {
  //todo disable timerfd_channel
}

TimerHandle TimerQueue::add_timer(TimerCallback cb, Timestamp when, int64_t interval_ms) {
  Timer::SharedPtr timer{new Timer(std::move(cb), when, interval_ms)};

  loop_->run_in_loop(std::bind(&TimerQueue::add_timer_in_loop, this, timer));
  return TimerHandle{timer};
}

void TimerQueue::add_timer_in_loop(Timer::SharedPtr timer) {
  loop_->assert_in_loop_thread();
  bool earliest_changed = insert(timer);
  if (earliest_changed) {
    timerfd_->reset(timer->get_expiration_ts());
  }
}

bool TimerQueue::insert(Timer::SharedPtr timer) {
  loop_->assert_in_loop_thread();
  assert(timer);
  bool earliest_changed = false;
  Timestamp when = timer->get_expiration_ts();
  if (timer_list_.empty() || when < timer_list_.begin()->first) {
    earliest_changed = true;
  }
  {
    timer_list_.insert({when, timer});
  }
  return earliest_changed;
}

std::vector<Timer::SharedPtr> TimerQueue::get_expired(Timestamp now) {
  loop_->assert_in_loop_thread();
  std::vector<Timer::SharedPtr> expired;
  while (!timer_list_.empty()) {
    if (timer_list_.begin()->first <= now) {
      expired.push_back(timer_list_.begin()->second);
      timer_list_.erase(timer_list_.begin());
    } else {
      break;
    }
  }
  return expired;
}

void TimerQueue::handle_read(Timestamp receive_time) {
  loop_->assert_in_loop_thread();
  Timestamp now = Timestamp::now();

  timerfd_->read();

  auto expired = get_expired(now);

  for (const auto &it : expired) {
    it->run();
  }

  reset();
}

void TimerQueue::reset() {
  //todo support repeat timers.
  loop_->assert_in_loop_thread();
  Timestamp new_timestamp{};
  if (!timer_list_.empty()) {
    new_timestamp = timer_list_.begin()->first;
    timerfd_->reset(new_timestamp);
  }
}

} // penduo