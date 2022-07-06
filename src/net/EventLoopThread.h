//
// Created by poilk on 2022/7/6.
//

#ifndef PENDUO_SRC_NET_EVENTLOOPTHREAD_H_
#define PENDUO_SRC_NET_EVENTLOOPTHREAD_H_

#include <thread>
#include <condition_variable>
#include "base/noncopyable.h"
#include "base/Mutex.h"

namespace penduo {

class EventLoop;

class EventLoopThread : noncopyable {
 public:
  EventLoopThread();
  ~EventLoopThread() = default;
  EventLoop *start_loop();

 private:
  void thread_func();

 private:
  Mutex mutex_;
  std::condition_variable cond_;
  EventLoop *loop_;
  std::thread thread_;
};

} // penduo

#endif //PENDUO_SRC_NET_EVENTLOOPTHREAD_H_
