//
// Created by poilk on 2022/7/7.
//

#ifndef PENDUO_SRC_NET_INETADDRESS_H_
#define PENDUO_SRC_NET_INETADDRESS_H_

#include <cstdint>
#include <netinet/in.h>
#include <string>

#include "SocketsOps.h"
#include "base/copyable.h"

namespace penduo {

// wrapper of sockaddr_in

class InetAddress : copyable{
 public:
  explicit InetAddress(uint16_t port = 0, bool loopback_only = false, bool ipv6 = false);

  sa_family_t family() const;

  const struct sockaddr * get_socket_addr() const {return sockets::sockaddr_cast(&addr_);}

  void set_socket_addr_inet6(sockaddr_in6 &addr){addr6_ = addr;}

  std::string to_ip();
  std::string to_ip_port();

 private:
  //distinguish ipv4 ipv6 by sin_family
  union{
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

} // penduo

#endif //PENDUO_SRC_NET_INETADDRESS_H_
