//
// Created by poilk on 2022/6/28.
//

#include <sys/timerfd.h>
#include <cassert>

#include "base/logging/Logger.h"
#include "net/Timer.h"
#include "net/TimerHandle.h"
#include "net/TimerQueue.h"
#include "net/EventLoop.h"

namespace penduo {

int create_timerfd() {
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

  if (timerfd < 0) {
    LOG_SYS_FATAL << "failed in timerfd_create";
  }
  return timerfd;
}

struct timespec how_much_time_from_now(Timestamp when) {
  auto now = Timestamp::now();
  auto microseconds = Timestamp::differ<Timestamp::TsMicroseconds>(now, when);
  microseconds = std::max(microseconds, static_cast<decltype(microseconds)>(100));
  struct timespec ts{};
  ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::MicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>((microseconds % Timestamp::MicroSecondsPerSecond) * 1000);
  return ts;
}

void reset_timerfd(int timerfd, Timestamp expiration) {
  //wake up loop by timerfd_settime()

  struct itimerspec new_value{};
  new_value.it_value = how_much_time_from_now(expiration);
  LOG_TRACE << "timerfd_settime set value: " << new_value.it_value.tv_sec << ' ' << new_value.it_value.tv_nsec
            << " interval: " << new_value.it_interval.tv_sec << ' ' << new_value.it_interval.tv_nsec;
  int ret = ::timerfd_settime(timerfd, 0, &new_value, nullptr);
  if (ret) {
    LOG_SYS_ERROR << "timerfd_settime()";
  }
}

void read_timerfd(int timerfd, Timestamp now) {
  uint64_t howmany;
  size_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handle_read() " << howmany << " at " << now.to_string();
  if (n != sizeof howmany) {
    LOG_ERROR << "TimerQueue::handle_read() reads " << n << " bytes instead of 8";
  }
}

TimerQueue::TimerQueue(EventLoop *loop) :
    loop_(loop),
    timerfd_(create_timerfd()),
    timerfd_channel_(loop_, timerfd_) {
  timerfd_channel_.set_read_callback(std::bind(&TimerQueue::handle_read, this));
  timerfd_channel_.enable_reading();
}

TimerQueue::~TimerQueue() {
  //todo disable timerfd_channel
  ::close(timerfd_);
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
    reset_timerfd(timerfd_, timer->get_expiration_ts());
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

void TimerQueue::handle_read() {
  loop_->assert_in_loop_thread();
  Timestamp now = Timestamp::now();

  read_timerfd(timerfd_, now);

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
    reset_timerfd(timerfd_, new_timestamp);
  }
}

} // penduo