//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_NET_CHANNEL_H_
#define PENDUO_SRC_NET_CHANNEL_H_

#include <cstdint>
#include <functional>

#include "base/noncopyable.h"
#include "base/Timestamp.h"


namespace penduo {

class EventLoop;

// A selectable I/O channel
// This class doesn't own the file descriptor.
// The file descriptor could be a socket, an event_fd, a timer_fd or a signal_fd.
class Channel : noncopyable{
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(Timestamp)> ReadEventCallback;

  Channel(EventLoop *loop, int fd);
  ~Channel();

  void handle_event(Timestamp receive_time);
  void set_read_callback(const ReadEventCallback &cb){read_callback_ = cb;}
  void set_write_callback(const EventCallback &cb){write_callback_ = cb;}
  void set_error_callback(const EventCallback &cb){error_callback_ = cb;}
  void set_close_callback(const EventCallback &cb){close_callback_ = cb;}

  int fd() const { return fd_;}
  int events() const {return events_;}
  void set_revents(int revents) {revents_ = revents;}
  bool is_none_event() const {return events_ == K_NONE_EVENT;}

  void enable_reading() {events_ |= K_READ_EVENT; update();}
  void disable_all() {events_ = K_NONE_EVENT; update();}

  void remove();

  // for poller
  //todo encapsulation
  int get_pollfds_index() const{return pollfds_index_;}
  void set_pollfds_index(int idx){pollfds_index_ = idx;}

  EventLoop *owner_loop(){return loop_;}

 private:
  void update();

  static const int K_NONE_EVENT;
  static const int K_READ_EVENT;
  static const int K_WRITE_EVENT;

  EventLoop *loop_;
  int fd_;
  int events_;
  int revents_;
  int pollfds_index_; //used by poller

  ReadEventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback error_callback_;
  EventCallback close_callback_;
  bool event_handling_;
  bool added_to_loop_;
};

} // penduo

#endif //PENDUO_SRC_NET_CHANNEL_H_
