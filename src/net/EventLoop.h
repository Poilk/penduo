//
// Created by poilk on 2022/6/27.
//

#ifndef PENDUO_SRC_KERNEL_EVENTLOOP_H_
#define PENDUO_SRC_KERNEL_EVENTLOOP_H_

#include <thread>
#include <vector>
#include <atomic>

#include "base/noncopyable.h"

namespace penduo{

class Channel;
class Poller;

class EventLoop : noncopyable {
 public:
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

  void quit();

 private:
  void abort_not_in_loop_thread();

  typedef std::vector<Channel*> ChannelList;

  std::unique_ptr<Poller> poller_;
  std::atomic<bool> looping_;
  std::atomic<bool> quit_;
  const std::thread::id thread_id_;
  ChannelList  active_channels_;
};

} //namespace penduo


#endif //PENDUO_SRC_KERNEL_EVENTLOOP_H_
