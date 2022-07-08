//
// Created by poilk on 2022/7/7.
//

#include <cassert>

#include "InetAddress.h"
#include "net/SocketsOps.h"
#include "net/Endian.h"

namespace penduo {

InetAddress::InetAddress(uint16_t port, bool loopback_only, bool ipv6) {
  static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
  static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");
  if (ipv6) {
    addr6_ = {};
    addr6_.sin6_family = AF_INET6;
    in6_addr ip = loopback_only ? in6addr_loopback : in6addr_any;
    addr6_.sin6_addr = ip;
    addr6_.sin6_port = sockets::host_to_network16(port);
  } else {
    addr_ = {};
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopback_only ? INADDR_LOOPBACK : INADDR_ANY;
    addr_.sin_addr.s_addr = sockets::host_to_network32(ip);
    addr_.sin_port = sockets::host_to_network16(port);
  }
}

sa_family_t InetAddress::family() const {
  return addr_.sin_family;
}

std::string InetAddress::to_ip() {
  return sockets::to_ip(get_socket_addr());
}

std::string InetAddress::to_ip_port() {
  return sockets::to_ip_port(get_socket_addr());
}

} // penduo