//
// Created by poilk on 2022/7/9.
//

#include <unistd.h>
#include <cassert>
#include "TcpConnection.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Socket.h"

namespace penduo {

TcpConnection::TcpConnection(EventLoop *loop,
                             std::string name,
                             int socket_fd,
                             InetAddress local_addr,
                             InetAddress peer_addr)
    : loop_(loop),
      name_(std::move(name)),
      state_(Connecting),
      socket_(new Socket(socket_fd)),
      channel_(new Channel(loop, socket_->fd())),
      local_addr_(local_addr),
      peer_addr_(peer_addr) {
  channel_->set_read_callback(std::bind(&TcpConnection::handle_read, this));
}

void TcpConnection::connect_establish() {
  loop_->assert_in_loop_thread();
  assert(state_ == Connecting);
  set_state(Connected);
  assert(state_ == Connected);
  channel_->enable_reading();

  auto shared_this = shared_from_this();
  connection_callback_(shared_this);
}

void TcpConnection::handle_read() {
  char buf[65536]{};
  size_t n = ::read(channel_->fd(), buf, sizeof buf);
  message_callback_(shared_from_this(), buf, n);
  // todo close connection if n == 0
}
} // penduo