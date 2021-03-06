//
// Created by poilk on 2022/6/27.
//

#include <cassert>
#include <thread>
#include <sys/poll.h>
#include <unistd.h>

#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Poller.h"
#include "net/TimerQueue.h"
#include "base/logging/Logger.h"

namespace penduo {

const int K_POLLTIME_MS = 10000;

thread_local EventLoop *t_loop_in_this_thread = nullptr;

EventLoop::EventLoop() :
    poller_(new Poller(this)),
    looping_(false),
    quit_(false),
    thread_id_(std::this_thread::get_id()),
    timer_queue_(new TimerQueue(this)),
    wakeup_pipe_(),
    wakeup_channel_(),
    calling_pending_functors_(false){
  if (t_loop_in_this_thread) {
    LOG_FATAL << "Another EventLoop " << t_loop_in_this_thread << " exists in this thread " << thread_id_;
  } else {
    t_loop_in_this_thread = this;
  }
  int ret = ::pipe(wakeup_pipe_);
  assert(ret == 0);
  wakeup_channel_ = std::unique_ptr<Channel>(new Channel(this, wakeup_pipe_[0]));
  wakeup_channel_->set_read_callback(std::bind(&EventLoop::wakeupfd_handle_read, this, std::placeholders::_1));
  wakeup_channel_->enable_reading();
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
  quit_ = false;

  while (!quit_) {
    active_channels_.clear();
    poller_->poll(K_POLLTIME_MS, active_channels_);
    Timestamp receive_time = Timestamp::now();
    for (auto channel : active_channels_) {
      channel->handle_event(receive_time);
    }
    do_pending_functors();
  }
  LOG_TRACE << "EventLoop " << this << " tid " << thread_id_ << " stop looping";

  looping_ = false;
}

void EventLoop::run_in_loop(Functor cb) {
  if(is_in_loop_thread()){
    LOG_TRACE << "run_in_loop, in EventLoop thread, directly run it.";
    cb();
  } else{
    LOG_TRACE << "run_in_loop, not in EventLoop thread, queue functor in pending queue.";
    queue_in_loop(std::move(cb));
  }
}

void EventLoop::queue_in_loop(Functor cb) {
  {
    LockGuard lock_guard(mutex_);
    pending_functors_.push_back(std::move(cb));
  }

  if(!is_in_loop_thread() || calling_pending_functors_){
    wakeup();
  }
}

TimerHandle EventLoop::run_at(Timestamp timestamp, TimerCallback cb) {
  return timer_queue_->add_timer(std::move(cb), timestamp);
}

TimerHandle EventLoop::run_after(int64_t delay_ms, TimerCallback cb) {
  Timestamp when = Timestamp::add<Timestamp::TsMilliseconds>(Timestamp::now(), delay_ms);
  return timer_queue_->add_timer(std::move(cb), when);
}

void EventLoop::do_pending_functors() {
  assert_in_loop_thread();
  calling_pending_functors_ = true;
  std::vector<Functor> functors;
  {
    LockGuard lock_guard(mutex_);
    std::swap(functors, pending_functors_);
  }
  for( const Functor &func : functors){
    func();
  }
  calling_pending_functors_ = false;
}

void EventLoop::wakeup() {
  int64_t one{};
  ::write(wakeup_pipe_[1], &one, sizeof one);
}

void EventLoop::wakeupfd_handle_read(Timestamp receive_time) {
  int64_t one{};
  ::read(wakeup_pipe_[0], &one, sizeof one);
}

void EventLoop::quit() {
  quit_ = true;

  if (!is_in_loop_thread()) {
    wakeup();
  }
}

void EventLoop::update_channel(Channel *channel) {
  assert(channel->owner_loop() == this);
  assert_in_loop_thread();
  poller_->update_channel(channel);
}

void EventLoop::remove_channel(Channel *channel) {
  assert(channel->owner_loop() == this);
  assert_in_loop_thread();
  //todo event_handling
  poller_->remove_channel(channel);
}

} //namespace penduo