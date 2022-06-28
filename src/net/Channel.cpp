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
    pollfds_index_(-1){
  assert(loop);
}

void Channel::update() {
  assert(loop_);
  loop_->update_channel(this);
}

void Channel::handle_event() {
  //todo fix handle with guard ?
  if (revents_ & POLLNVAL) {
    LOG_WARNING << "Channel::handle_event() POLLNVAL";
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
}

} // penduo