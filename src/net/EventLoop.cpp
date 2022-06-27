//
// Created by poilk on 2022/6/27.
//

#include "EventLoop.h"
#include <thread>
#include <sys/poll.h>

namespace penduo{

thread_local EventLoop * t_loop_in_this_thread = nullptr;

EventLoop::EventLoop() : looping_(false), thread_id_(std::this_thread::get_id()){
  if(t_loop_in_this_thread){
    //todo logging
    exit(-1);
  } else{
    t_loop_in_this_thread = this;
  }
};

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loop_in_this_thread = nullptr;
}

void EventLoop::abort_not_in_loop_thread() {
  //todo logging and exit
  exit(-1);
}

EventLoop *EventLoop::get_event_loop_of_current_thread() {
  return t_loop_in_this_thread;
}

void EventLoop::loop() {
  assert(!looping_);
  assert_in_loop_thread();
  looping_ = true;

  ::poll(nullptr, 0, 5 * 1000);

  looping_ = false;
}

} //namespace penduo