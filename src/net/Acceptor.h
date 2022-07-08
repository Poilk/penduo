//
// Created by poilk on 2022/7/7.
//

#ifndef PENDUO_SRC_NET_ACCEPTOR_H_
#define PENDUO_SRC_NET_ACCEPTOR_H_

#include <functional>

#include "base/noncopyable.h"
#include "net/Channel.h"
#include "net/Socket.h"

namespace penduo {

class EventLoop;
class InetAddress;

class Acceptor : noncopyable{
 public:
  typedef std::function<void (int socket_fd, const InetAddress &)> NewConnectionCallback;

  Acceptor(EventLoop *loop, const InetAddress & listen_addr, bool reuse_port);

  void set_new_connection_callback(const NewConnectionCallback &cb){
    new_connection_callback_ = cb;
  }

  bool listening() const {return listening_;}
  void listen();

 private:
  void handle_read();

 private:

  EventLoop *loop_;
  Socket accept_socket_;
  Channel accept_channel_;
  bool listening_;
  NewConnectionCallback new_connection_callback_;
};

} // penduo

#endif //PENDUO_SRC_NET_ACCEPTOR_H_
