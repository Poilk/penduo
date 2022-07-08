//
// Created by poilk on 2022/7/7.
//

#include <sys/socket.h>

#include "Socket.h"
#include "net/InetAddress.h"
#include "net/SocketsOps.h"

namespace penduo {

void Socket::bind_address(const InetAddress &local_addr) {
  sockets::bind_or_die(socket_fd_, local_addr.get_socket_addr());
}

Socket::~Socket() {
  sockets::close(socket_fd_);
}

void Socket::set_reuse_addr(bool on) {
  int option_val = on ? 1 : 0;
  auto option_len = static_cast<socklen_t>(sizeof option_val);
  ::setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &option_val, option_len);
  //todo check?
}

void Socket::listen() {
  sockets::listen_or_die(socket_fd_);
}

int Socket::accept(InetAddress *peer_addr) {
  struct sockaddr_in6 addr{};
  int connect_fd = sockets::accept(socket_fd_, &addr);
  if (connect_fd >= 0){
    peer_addr->set_socket_addr_inet6(addr);
  }
  return connect_fd;
}

} // penduo