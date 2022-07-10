//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_NET_POLLER_H_
#define PENDUO_SRC_NET_POLLER_H_

#include <vector>
#include <map>

#include "base/noncopyable.h"

struct pollfd;

namespace penduo {

class Channel;
class EventLoop;

//
// IO Multiplexing with poll
// This class doesn't own the Channel objects.
//
class Poller : noncopyable{
 public:
  typedef std::vector<Channel *> ChannelList;

  Poller(EventLoop *loop);
  ~Poller();

  // Polls the I/O events.
  // Must be called in the loop thread.
  //todo return timestamp
  void poll(int timeout_ms, ChannelList &active_channels);

  // Change the interested I/O events
  // Must be called in the loop thread.
  void update_channel(Channel *channel);

  void remove_channel(Channel *channel);

  void assert_in_loop_thread();

 private:
  void fill_active_channels(int num_events, ChannelList &active_channels);

  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop *owner_loop_;
  PollFdList pollfds_;
  ChannelMap channels_;
};

} // penduo

#endif //PENDUO_SRC_NET_POLLER_H_
