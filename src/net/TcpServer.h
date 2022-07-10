//
// Created by poilk on 2022/7/8.
//

#ifndef PENDUO_SRC_NET_TCPSERVER_H_
#define PENDUO_SRC_NET_TCPSERVER_H_

#include <string>
#include <memory>
#include <map>

#include "base/noncopyable.h"
#include "net/Callbacks.h"
#include "net/InetAddress.h"

namespace penduo {

class EventLoop;
class Acceptor;

class TcpServer : noncopyable {
 public:
  TcpServer(EventLoop *loop, const InetAddress &listen_addr, std::string name);
  ~TcpServer();

  // Starts the server if it's not listening.
  //
  // it's harmless to call it multiple times.
  // thread safe
  void start();

  // Set connection callback.
  // Not thread safe.
  void set_connection_callback(const ConnectionCallback &cb) {connection_callback_ = cb;}

  // Set message callback.
  // Not thread sage.
  void set_message_callback(const MessageCallback &cb) {message_callback_ = cb;}


 private:
  // Not thread safe, but in loop
  void new_connection(int socket_fd, const InetAddress &peer_addr);

  void remove_connection(const TcpConnectionPtr &connection);

  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

  EventLoop *loop_;
  const std::string name_;
  std::unique_ptr<Acceptor> acceptor_;
  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;
  CloseCallback close_callback_;
  bool started_;
  int next_connect_id_;
  ConnectionMap connections_;
};

} // penduo

#endif //PENDUO_SRC_NET_TCPSERVER_H_
