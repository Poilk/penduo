//
// Created by poilk on 2022/7/4.
//

#include <sys/timerfd.h>
#include <unistd.h>

#include "TimerfdNaive.h"
#include "base/logging/Logger.h"

namespace penduo {

TimerfdNaive::TimerfdNaive() : fd_(-1){
  fd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

  if (fd_ < 0) {
    LOG_SYS_FATAL << "failed in timerfd_create";
  }
}

TimerfdNaive::~TimerfdNaive() {
  ::close(fd_);
}

void TimerfdNaive::read() {
  Timestamp now = Timestamp::now();
  uint64_t howmany;
  size_t n = ::read(fd_, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handle_read() " << howmany << " at " << now.to_string();
  if (n != sizeof howmany) {
    LOG_ERROR << "TimerQueue::handle_read() reads " << n << " bytes instead of 8";
  }
}

void TimerfdNaive::reset(Timestamp expiration) {
  //wake up loop by timerfd_settime()

  struct itimerspec new_value{};
  new_value.it_value = how_much_time_from_now(expiration);
  LOG_TRACE << "timerfd_settime set value: " << new_value.it_value.tv_sec << ' ' << new_value.it_value.tv_nsec
            << " interval: " << new_value.it_interval.tv_sec << ' ' << new_value.it_interval.tv_nsec;
  int ret = ::timerfd_settime(fd_, 0, &new_value, nullptr);
  if (ret) {
    LOG_SYS_ERROR << "timerfd_settime()";
  }
}

} // penduo