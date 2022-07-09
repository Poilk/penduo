//
// Created by poilk on 2022/7/8.
//

#include "TcpServer.h"
#include "base/logging/Logger.h"
#include "net/EventLoop.h"
#include "net/Acceptor.h"
#include "net/TcpConnection.h"

namespace penduo {

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listen_addr, std::string name) :
    loop_(loop),
    name_(std::move(name)),
    acceptor_(new Acceptor(loop_, listen_addr, true)),
    started_(false),
    next_connect_id_(0) {

  acceptor_->set_new_connection_callback(std::bind(&TcpServer::new_connection,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2));
}

TcpServer::~TcpServer() = default;

void TcpServer::start() {
  loop_->assert_in_loop_thread();
  started_ = true;
  acceptor_->listen();
}

void TcpServer::new_connection(int socket_fd, const InetAddress &peer_addr) {
  loop_->assert_in_loop_thread();

  std::string connection_name = name_ + "#" + std::to_string(++next_connect_id_);
  LOG_INFO << "TcpServer::newConnection [" << name_ << "] - new connection [" << connection_name
           << "] from " << peer_addr.to_ip_port();

  InetAddress local_addr(sockets::get_local_addr(socket_fd));
  // todo poll with zero timeout to double confirm the new connection
  TcpConnectionPtr
      connection = std::make_shared<TcpConnection>(loop_, connection_name, socket_fd, local_addr, peer_addr);
  connections_[connection_name] = connection;
  connection->set_connection_callback(connection_callback_);
  connection->set_message_callback(message_callback_);
  connection->connect_establish();
}

} // penduo