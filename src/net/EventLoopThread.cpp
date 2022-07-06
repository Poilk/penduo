//
// Created by poilk on 2022/7/6.
//

#include "EventLoopThread.h"
#include "net/EventLoop.h"

namespace penduo {

EventLoopThread::EventLoopThread() :
    loop_(nullptr),
    thread_(&EventLoopThread::thread_func, this) {
}

EventLoop *EventLoopThread::start_loop() {
  std::unique_lock<Mutex> lock(mutex_);
  cond_.wait(lock, [this]{return loop_;});
  return loop_;
}

void EventLoopThread::thread_func() {
  EventLoop loop;
  {
    LockGuard lock_guard(mutex_);
    loop_ = &loop;
    cond_.notify_one();
  }

  loop.loop();
}

} // penduo