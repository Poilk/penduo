//
// Created by poilk on 2022/6/27.
//

#ifndef PENDUO_SRC_KERNEL_EVENTLOOP_H_
#define PENDUO_SRC_KERNEL_EVENTLOOP_H_

#include <thread>
#include <vector>
#include <atomic>
#include <functional>

#include "base/Mutex.h"
#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "net/TimerHandle.h"
#include "net/Callbacks.h"

namespace penduo{

class Channel;
class Poller;
class TimerQueue;

class EventLoop : noncopyable {
 public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();

  void loop();

  static EventLoop *get_event_loop_of_current_thread();

  void update_channel(Channel *channel);
  void remove_channel(Channel *Channel);

  void assert_in_loop_thread(){
    if (!is_in_loop_thread()){
      abort_not_in_loop_thread();
    }
  }

  bool is_in_loop_thread() const {
    return thread_id_ == std::this_thread::get_id();
  }

  void run_in_loop(Functor cb);


  TimerHandle run_at(Timestamp timestamp, TimerCallback cb);
  TimerHandle run_after(int64_t delay_ms, TimerCallback cb);
  //TimerHandle run_every(int64_t interval_ms, TimerCallback cb);

  void quit();

 private:
  void abort_not_in_loop_thread();
  void queue_in_loop(Functor cb);
  void do_pending_functors();

  typedef std::vector<Channel*> ChannelList;

  std::unique_ptr<Poller> poller_;
  std::atomic<bool> looping_;
  std::atomic<bool> quit_;
  const std::thread::id thread_id_;
  std::unique_ptr<TimerQueue> timer_queue_;
  ChannelList  active_channels_;

  mutable Mutex mutex_;
  std::vector<Functor> pending_functors_;
};

} //namespace penduo


#endif //PENDUO_SRC_KERNEL_EVENTLOOP_H_
