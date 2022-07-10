//
// Created by poilk on 2022/7/9.
//

#include <unistd.h>
#include <cassert>
#include "TcpConnection.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Socket.h"
#include "base/logging/Logger.h"

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
  channel_->set_read_callback(std::bind(&TcpConnection::handle_read, this, std::placeholders::_1));
}

void TcpConnection::connect_establish() {
  loop_->assert_in_loop_thread();
  assert(state_ == Connecting);
  set_state(Connected);
  assert(state_ == Connected);
  channel_->enable_reading();

  connection_callback_(shared_from_this());
}

void TcpConnection::connect_destoryed() {
  loop_->assert_in_loop_thread();
  assert(state_ == Connected);
  set_state(DisConnected);
  channel_->disable_all();

  connection_callback_(shared_from_this());

  channel_->remove();
}

void TcpConnection::handle_read(Timestamp receive_time) {
  int saved_errno;
  size_t n = input_buffer_.read_fd(channel_->fd(), &saved_errno);
  if (n > 0) {
    message_callback_(shared_from_this(), &input_buffer_, receive_time);
  } else if (n == 0) {
    handle_close();
  } else {
    LOG_SYS_ERROR << "TcpConnection handle_read: " << saved_errno;
    handle_error();
  }

}

void TcpConnection::handle_close() {
  loop_->assert_in_loop_thread();
  LOG_TRACE << "TcpConnect::handle_close state = " << state_;
  assert(state_ == Connected);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  channel_->disable_all();
  if (close_callback_) {
    //why????
    //must be the last line;
    close_callback_();
  }
}

void TcpConnection::handle_error() {
  int error = sockets::get_socket_error(socket_->fd());
  LOG_ERROR << "TcpConnection::handle_error [" << name_ << "] - SO_ERROR = " << error << " " << strerror_tl(error);
}

} // penduo