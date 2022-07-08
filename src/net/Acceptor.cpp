//
// Created by poilk on 2022/7/7.
//

#include "Acceptor.h"
#include "net/EventLoop.h"
#include "net/SocketsOps.h"
#include "net/InetAddress.h"

namespace penduo {

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listen_addr, bool reuse_port) :
    loop_(loop),
    accept_socket_(sockets::create_nonblocking_or_die(listen_addr.family())),
    accept_channel_(loop_, accept_socket_.fd()),
    listening_(false) {
  accept_socket_.set_reuse_addr(reuse_port);
  accept_socket_.bind_address(listen_addr);
  accept_channel_.set_read_callback(std::bind(&Acceptor::handle_read, this));
}

void Acceptor::listen() {
  loop_->assert_in_loop_thread();
  listening_ = true;
  accept_socket_.listen();
  accept_channel_.enable_reading();
}

void Acceptor::handle_read() {
  loop_->assert_in_loop_thread();
  InetAddress peer_addr{};
  //todo loop until no more;
  //todo not use naive socket fd?
  int connect_fd = accept_socket_.accept(&peer_addr);
  if(connect_fd >= 0){
    if(new_connection_callback_){
      new_connection_callback_(connect_fd, peer_addr);
    } else{
      sockets::close(connect_fd);
    }
  }
}

} // penduo