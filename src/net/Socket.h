//
// Created by poilk on 2022/7/7.
//

#ifndef PENDUO_SRC_NET_SOCKET_H_
#define PENDUO_SRC_NET_SOCKET_H_

#include "base/noncopyable.h"

namespace penduo {

class InetAddress;

class Socket : noncopyable{
 public:
  explicit Socket(int socket_fd) : socket_fd_(socket_fd){
  }

  ~Socket();

  int fd()const{return socket_fd_;}

  void set_reuse_addr(bool on);

  void bind_address(const InetAddress &local_addr);
  void listen();
  int accept(InetAddress * peer_addr);

 private:
  const int socket_fd_;
};

} // penduo

#endif //PENDUO_SRC_NET_SOCKET_H_
