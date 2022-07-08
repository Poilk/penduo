//
// Created by poilk on 2022/7/8.
//

#include <iostream>
#include <string>
#include <unistd.h>

#include "net/InetAddress.h"
#include "net/SocketsOps.h"
#include "net/EventLoop.h"
#include "net/Acceptor.h"

using namespace penduo;

void new_connect(int socket_fd, InetAddress peer_addr){
  std::cout << " new_connect(): accepted a new connection from " << peer_addr.to_ip_port() << std::endl;
  std::string write_string = "Hello penduo!\n";
  ::write(socket_fd, write_string.data(), write_string.size());
  sockets::close(socket_fd);
}


int main(int argc, char *argv[]){
  std::cout << "main(): pid = " << getpid() << std::endl;

  std::cout << sizeof (struct sockaddr_in6) << ' ' << sizeof (struct sockaddr_in) << std::endl;

  InetAddress listen_addr(9981);
  std::cout << listen_addr.to_ip_port() << std::endl;
  EventLoop loop;

  Acceptor acceptor(&loop, listen_addr, true);
  acceptor.set_new_connection_callback(new_connect);
  acceptor.listen();

  loop.loop();

  return 0;
}