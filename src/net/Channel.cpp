//
// Created by poilk on 2022/6/28.
//

#include <poll.h>
#include <cassert>

#include "Channel.h"
#include "EventLoop.h"
#include "base/logging/Logger.h"

namespace penduo {

const int Channel::K_NONE_EVENT = 0;
const int Channel::K_READ_EVENT = POLLIN | POLLPRI;
const int Channel::K_WRITE_EVENT = POLLOUT;

Channel::Channel(EventLoop *loop, int fd) :
    loop_(loop),
    fd_(fd),
    events_(K_NONE_EVENT),
    revents_(K_NONE_EVENT),
    pollfds_index_(-1),
    event_handling_(false),
    added_to_loop_(false){
  assert(loop);
}

Channel::~Channel() {
  assert(!event_handling_);
  assert(!added_to_loop_);
  assert(is_none_event());
}

void Channel::remove() {
  loop_->assert_in_loop_thread();
  assert(is_none_event());
  assert(added_to_loop_);
  added_to_loop_ = false;
  loop_->remove_channel(this);
}

void Channel::update() {
  loop_->assert_in_loop_thread();
  assert(loop_);
  loop_->update_channel(this);
  added_to_loop_ = true;
}

void Channel::handle_event() {
  //todo fix handle with guard ?
  event_handling_ = true;
  if (revents_ & POLLNVAL) {
    LOG_WARNING << "Channel::handle_event() POLLNVAL";
  }

  if((revents_ & POLLHUP) && !(revents_ & POLLIN)){
    LOG_WARNING << "Channel::handle_event() POLLHUP";
    if(close_callback_){
      close_callback_();
    }
  }

  if (revents_ & (POLLERR | POLLNVAL)) {
    if (error_callback_) {
      error_callback_();
    }
  }
  //todo fix POLLRDHUP
  if (revents_ & (POLLIN | POLLPRI /*| POLLRDHUP*/)) {
    if (read_callback_) {
      read_callback_();
    }
  }

  if (revents_ & (POLLOUT)) {
    if (write_callback_) {
      write_callback_();
    }
  }
  event_handling_ = false;
}

} // penduo