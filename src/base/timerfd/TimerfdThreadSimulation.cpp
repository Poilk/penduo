//
// Created by poilk on 2022/7/5.
//

#include <unistd.h>
#include <thread>
#include <set>
#include <poll.h>

#include "base/noncopyable.h"
#include "base/Mutex.h"
#include "base/Timestamp.h"
#include "base/logging/Logger.h"
#include "TimerfdThreadSimulation.h"

namespace penduo {

class TimerThread : noncopyable {
 public:
  static TimerThread &get_instance() {
    static TimerThread instance;
    return instance;
  }
  void reset(int write_fd, Timestamp new_timestamp, Timestamp old_timestamp = {});
 private:
  void TimerFunc();
  TimerThread();
  ~TimerThread();

 private:
  Mutex mutex_;
  std::set<std::pair<Timestamp, int>> timer_set_;
  int weakup_fds_[2];
  volatile bool thread_running;
  std::condition_variable cond_;
  std::thread thread_;
};

TimerThread::TimerThread() :
    weakup_fds_(),
    thread_running(false),
    thread_(&TimerThread::TimerFunc, this){
  int ret = pipe(weakup_fds_);
  assert(ret == 0);
  std::unique_lock<Mutex> lock(mutex_);
  cond_.wait(lock, [&]{return thread_running;});
  LOG_TRACE << "TimerThread create pipe: " << weakup_fds_[0] << ' ' << weakup_fds_[1];
}

TimerThread::~TimerThread() {
  {
    LockGuard lock_guard(mutex_);
    thread_running = false;
  }
  thread_.join();
  ::close(weakup_fds_[0]);
  ::close(weakup_fds_[1]);
}

void TimerThread::reset(int write_fd, Timestamp new_timestamp, Timestamp old_timestamp) {
  LockGuard lock_guard(mutex_);
  if (old_timestamp.valid()) {
    timer_set_.erase({old_timestamp, write_fd});
  }
  timer_set_.insert({new_timestamp, write_fd});
  int64_t weak_fd_write_buf;
  ::write(weakup_fds_[1], &weak_fd_write_buf, sizeof weak_fd_write_buf);
}

void TimerThread::TimerFunc() {
  struct pollfd fd_array{};
  fd_array.fd = weakup_fds_[0];
  fd_array.events = POLLIN | POLLPRI;
  {
    LockGuard lock_guard(mutex_);
    thread_running = true;
    cond_.notify_one();
  }

  for (;;) {
    int64_t wait_ms = 10000;
    {
      LockGuard lock_guard(mutex_);
      if(!thread_running){
        break;
      }
      if (!timer_set_.empty()) {
        auto now = Timestamp::now();
        wait_ms = std::max(int64_t{1},
                           Timestamp::differ<Timestamp::TsMilliseconds>(now, timer_set_.begin()->first));
      }
    }
    int ret = ::poll(&fd_array, 1, static_cast<int>(wait_ms));
    auto now = Timestamp::now();
    if (ret >= 0) {
      if(ret == 0){
        //LOG_TRACE << "poll timeout , wait_ms: " << wait_ms;
      } else{
        //LOG_TRACE << "poll return " << ret << " events";
        int weak_fd_read_buf;
        ::read(fd_array.fd, &weak_fd_read_buf, sizeof weak_fd_read_buf);
      }
      std::vector<int> expiration_fds_;
      {
        LockGuard lock_guard(mutex_);
        while(!timer_set_.empty() && now >= timer_set_.begin()->first){
          expiration_fds_.push_back(timer_set_.begin()->second);
          timer_set_.erase(timer_set_.begin());
        }
      }
      for(auto each_fd : expiration_fds_){
        int64_t timer_write_buf{};
        ::write(each_fd, &timer_write_buf, sizeof timer_write_buf );
        LOG_TRACE << "TimerThread fd: " << each_fd << " write";
      }
    } else if (ret < 0) {
      LOG_SYS_ERROR << "poll " << fd_array.fd << " ret = " << ret << " errno = " << errno;
    } else if (ret == 0){
    }
  }
}

TimerfdThreadSimulation::TimerfdThreadSimulation() : pipe_fd_(), expiration_ts_(Timestamp{}){
  int ret = ::pipe(pipe_fd_);
  assert(ret == 0);
  LOG_TRACE << "TimerfdThreadSimulation create pipe: " << pipe_fd_[0] << ' ' << pipe_fd_[1];
}

TimerfdThreadSimulation::~TimerfdThreadSimulation() noexcept {
  ::close(pipe_fd_[0]);
  ::close(pipe_fd_[1]);
}

void TimerfdThreadSimulation::reset(Timestamp expiration) {
  auto old = expiration_ts_;
  TimerThread::get_instance().reset(pipe_fd_[1], expiration, old);
  expiration_ts_ = expiration;
}

void TimerfdThreadSimulation::read() {
  int64_t timer_write_buf{};
  auto read_size = ::read(pipe_fd_[0], &timer_write_buf, sizeof timer_write_buf );
  LOG_TRACE << "TimerfdThreadSimulation " << pipe_fd_[0] << " read " << read_size << " bytes";
}

int TimerfdThreadSimulation::get_fd() const {
  return pipe_fd_[0];
}

} // penduo