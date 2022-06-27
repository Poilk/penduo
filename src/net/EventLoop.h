//
// Created by poilk on 2022/6/27.
//

#ifndef PENDUO_SRC_KERNEL_EVENTLOOP_H_
#define PENDUO_SRC_KERNEL_EVENTLOOP_H_

#include <thread>
#include "noncopyable.h"

namespace penduo{

class EventLoop : noncopyable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();

  static EventLoop *get_event_loop_of_current_thread();

  void assert_in_loop_thread(){
    if (!is_in_loop_thread()){
      abort_not_in_loop_thread();
    }
  }

  bool is_in_loop_thread() const {
    return thread_id_ == std::this_thread::get_id();
  }

 private:
  void abort_not_in_loop_thread();

  bool looping_;
  const std::thread::id thread_id_;
};

} //namespace penduo


#endif //PENDUO_SRC_KERNEL_EVENTLOOP_H_
