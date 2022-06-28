//
// Created by poilk on 2022/6/27.
//

#include "EventLoop.h"
#include <thread>
#include <sys/poll.h>
#include "logging/Logger.h"

namespace penduo{

thread_local EventLoop * t_loop_in_this_thread = nullptr;

EventLoop::EventLoop() : looping_(false), thread_id_(std::this_thread::get_id()){
  if(t_loop_in_this_thread){
    LOG_FATAL << "Another EventLoop " << t_loop_in_this_thread << " exists in this thread " << thread_id_;
  } else{
    t_loop_in_this_thread = this;
  }
};

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loop_in_this_thread = nullptr;
}

void EventLoop::abort_not_in_loop_thread() {
  LOG_FATAL << "EventLoop::abort_not_in_loop_thread " << this << " was created in thread_id_ " << thread_id_
            << ", current thread_id_ " << std::this_thread::get_id();
}

EventLoop *EventLoop::get_event_loop_of_current_thread() {
  return t_loop_in_this_thread;
}

void EventLoop::loop() {
  assert(!looping_);
  assert_in_loop_thread();
  looping_ = true;

  LOG_INFO << "poll in EventLoop::loop of thread_id " << thread_id_;
  ::poll(nullptr, 0, 5 * 1000);

  looping_ = false;
}

} //namespace penduo