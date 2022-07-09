//
// Created by poilk on 2022/7/7.
//

#ifndef PENDUO_SRC_NET_SOCKETSOPS_H_
#define PENDUO_SRC_NET_SOCKETSOPS_H_

#include <string>
#include <arpa/inet.h>

namespace penduo {

namespace sockets{

int create_nonblocking_or_die(sa_family_t family);
void bind_or_die(int socket_fd, const struct sockaddr *addr);
int accept(int socket_fd, struct sockaddr_in6 * addr);
void listen_or_die(int socket_fd);
void close(int socket_fd);

std::string to_ip(const sockaddr *addr);
std::string to_ip_port(const sockaddr *addr);

const struct sockaddr *sockaddr_cast(const struct sockaddr_in * addr);
const struct sockaddr *sockaddr_cast(const struct sockaddr_in6 * addr);
struct sockaddr *sockaddr_cast(struct sockaddr_in6 * addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

struct sockaddr_in6 get_local_addr(int socket_fd);
struct sockaddr_in6 get_peer_addr(int socket_fd);

} // sockets

} // penduo

#endif //PENDUO_SRC_NET_SOCKETSOPS_H_
