//
// Created by poilk on 2022/7/7.
//

#include <fcntl.h>
#include <unistd.h>
#include <cassert>

#include "SocketsOps.h"
#include "net/Endian.h"
#include "base/logging/Logger.h"

namespace penduo {

void set_nonblocking_and_close_on_exec(int socket_fd) {
  // non-blcok
  int flags = ::fcntl(socket_fd, F_GETFL, 0);
  flags += O_NONBLOCK;
  int ret = ::fcntl(socket_fd, F_SETFL, flags);
  if(ret < 0){
    LOG_SYS_ERROR << "::fcntl errno " << errno;
  }

  // close-on-exec
  flags = ::fcntl(socket_fd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl(socket_fd, F_SETFD, flags);
  if(ret < 0){
    LOG_SYS_ERROR << "::fcntl errno " << errno;
  }
}

void sockets::bind_or_die(int socket_fd, const struct sockaddr *addr) {
  int ret = 0;
  if(addr->sa_family == AF_INET){
    ret = ::bind(socket_fd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
  } else if(addr->sa_family == AF_INET6){
    ret = ::bind(socket_fd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
  } else{
    LOG_FATAL << "sockets::bind unknown sa_family";
  }
  if (ret < 0) {
    LOG_SYS_FATAL << "sockets::bind_or_die " << errno;
  }
}

int sockets::create_nonblocking_or_die(sa_family_t family) {
  int socket_fd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
  if (socket_fd < 0) {
    LOG_SYS_FATAL << "sockets::create_nonblocking_or_die ret: ";
  }
  set_nonblocking_and_close_on_exec(socket_fd);
  return socket_fd;
}

void sockets::listen_or_die(int socket_fd) {
  int ret = ::listen(socket_fd, SOMAXCONN);
  if (ret < 0) {
    LOG_SYS_FATAL << "sockets::listen_or_die";
  }
}

int sockets::accept(int socket_fd, struct sockaddr_in6 *addr) {
  auto addr_len = static_cast<socklen_t>(sizeof *addr);
  int connect_fd = ::accept(socket_fd, sockaddr_cast(addr), &addr_len);
  set_nonblocking_and_close_on_exec(connect_fd);
  if (connect_fd < 0) {
    int saved_errno = errno;
    LOG_SYS_ERROR << "::accept";
    //todo errno meaning?
    switch (saved_errno) {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO: // ???
      case EPERM:
      case EMFILE: // per-process lmit of open file desctiptor ???
        // expected errors
        errno = saved_errno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        // unexpected errors
      LOG_FATAL << "unexpected error of ::accept " << saved_errno;
        break;
      default:LOG_FATAL << "unknown error of ::accept " << saved_errno;
        break;
    }
  }
  return connect_fd;
}

void sockets::close(int socket_fd) {
  if (::close(socket_fd) < 0) {
    LOG_SYS_ERROR << "::close " << socket_fd << "failed, errno " << errno;
  }
}

std::string sockets::to_ip(const sockaddr *addr) {
  const size_t buf_size = 64;
  char buf[buf_size]{};
  if (addr->sa_family == AF_INET) {
    assert(buf_size >= INET_ADDRSTRLEN);
    const struct sockaddr_in *addr4 = sockaddr_in_cast(addr);
    ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(buf_size));
    return buf;
  } else if (addr->sa_family == AF_INET6) {
    assert(buf_size >= INET6_ADDRSTRLEN);
    const struct sockaddr_in6 *addr6 = sockaddr_in6_cast(addr);
    ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(buf_size));
    return buf;
  } else{
    LOG_FATAL << "unknown sa_family " << addr->sa_family;
    return "unknown sa_family";
  }
}

std::string sockets::to_ip_port(const sockaddr *addr) {
  if (addr->sa_family == AF_INET) {
    const struct sockaddr_in *addr4 = sockaddr_in_cast(addr);
    uint16_t port = sockets::network_to_host16(addr4->sin_port);
    return "[" + to_ip(addr) + "]:" + std::to_string(port);
  } else if (addr->sa_family == AF_INET6) {
    const struct sockaddr_in6 *addr6 = sockaddr_in6_cast(addr);
    uint16_t port = sockets::network_to_host16(addr6->sin6_port);
    return "[" + to_ip(addr) + "]:" + std::to_string(port);
  } else{
    LOG_FATAL << "unknown sa_family " << addr->sa_family;
    return "unknown sa_family";
  }
}

const struct sockaddr *sockets::sockaddr_cast(const struct sockaddr_in *addr) {
  return reinterpret_cast<const struct sockaddr *>(addr);
}

const struct sockaddr *sockets::sockaddr_cast(const struct sockaddr_in6 *addr) {
  return reinterpret_cast<const struct sockaddr *>(addr);
}

struct sockaddr *sockets::sockaddr_cast(struct sockaddr_in6 *addr) {
  return reinterpret_cast<struct sockaddr *>(addr);
}

const struct sockaddr_in6 *sockets::sockaddr_in6_cast(const struct sockaddr *addr) {
  return reinterpret_cast<const struct sockaddr_in6 *>(addr);
}

const struct sockaddr_in *sockets::sockaddr_in_cast(const struct sockaddr *addr) {
  return reinterpret_cast<const struct sockaddr_in *>(addr);
}

} // penduo